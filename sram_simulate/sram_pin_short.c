#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>

#define SRAM_SIZE (32*1024)
#define ADDRESS_PIN 15
#define IO_PINS 8
#define CONTROL_PINS 3
#define TOTAL_PINS (ADDRESS_PIN+IO_PINS+CONTROL_PINS+2)//包括VCC,GND


//引脚类型
typedef enum{
    PIN_TYPE_ADDRESS,
    PIN_TYPE_IO,
    PIN_TYPE_VCC,
    PIN_TYPE_GND,
    PIN_TYPE_CE,
    PIN_TYPE_OE,
    PIN_TYPE_WE
}PinType;

//引脚
typedef struct pin_str{
    int number;//引脚编号
    int index;//所属类型引脚的下标。如是adress引脚，是第0个adress引脚
    PinType type;//引脚类型
}pin_str;

//短路的引脚
int pin_1=0;
int pin_2=0;
//sram的缓冲区
uint8_t sram[SRAM_SIZE]={0};
//引脚表
pin_str pin_table[TOTAL_PINS];
//初始化引脚类型
void InitPinTypes()
{
    //1~10:ADDRESS  11~13:IO  14:GND  15~19:IO  20:CE
    //21:ADDRESS  22:OE  23~26:ADDRESS  27:WE 28:VCC
    //左侧：1~7，22~28  右侧：8~21
    for(int i=0;i<10;++i)
    {
        pinTypes[i]=PIN_TYPE_ADDRESS;
    }
    for(int i=10;i<13;++i)
    {
        pinTypes[i]=PIN_TYPE_IO;
    }
    pinTypes[13]=PIN_TYPE_GND;

    for(int i=14;i<19;++i)
    {
        pinTypes[i]=PIN_TYPE_IO;
    }
    pinTypes[19]=PIN_TYPE_CE;
    pinTypes[20]=PIN_TYPE_ADDRESS;
    pinTypes[21]=PIN_TYPE_OE;
    for(int i=22;i<26;++i)
    {
        pinTypes[i]=PIN_TYPE_ADDRESS;
    }
    pinTypes[26]=PIN_TYPE_WE;
    pinTypes[27]=PIN_TYPE_VCC;
}
//检查引脚类型
int check_pin_type(int pin1,int pin2)
{
    if((pin1<1||pin1>28)||(pin2<1||pin2>28))//pin1 pin2不在范围内
    {
        return 0;
    }

    if(pin1==pin2)//输入同一个pin脚
    {
        return 0;
    }
    //两个pin相隔距离远
    //左侧：1~7，22~28  右侧：8~21
    //pin1在左侧
    if((pin1>=1&&pin1<=7)||(pin1>=22&&pin1<=28))
    {
        if(pin2>=8&&pin2<=21)//pin2在右侧
        {
            return 0;
        }else{//pin2也在左侧
            if((pin1!=28&&pin2!=28)&&(abs(pin1-pin2)>1))//pin1 ，pin2间距大于1
            {
                return 0;
            }
        }
    }else //pin1在右侧
    {
        if((pin2>=1&&pin2<=7)||(pin2>=22&&pin2<=28))
        {
            return 0;
        }else{//pin2也在右侧
            if(abs(pin1-pin2)>1)//pin1 ，pin2间距大于1
            {
                return 0;
            }
        }
    }
    //输入为vcc与adress或gnd与address
    if((pinTypes[pin1-1]==PIN_TYPE_VCC||pinTypes[pin1-1]==PIN_TYPE_GND)
        &&pinTypes[pin2-1]==PIN_TYPE_ADDRESS)
    {
        return 1;
    }
    if((pinTypes[pin2-1]==PIN_TYPE_VCC||pinTypes[pin2-1]==PIN_TYPE_GND)
        &&pinTypes[pin1-1]==PIN_TYPE_ADDRESS)
    {
        return 1;
    }
    //输入pin类型不同
    if(pinTypes[pin1-1]!=pinTypes[pin2-1])
    {
        return 0;
    }
    
    return 1;
  
}

//2进制转换10进制
unsigned int binary_convert_decimal(const char *bin)
{
    unsigned int dec=0;
    int len=strlen(bin);
    int count=1;
    for(int i=len-1;i>=0;--i)
    {
        dec+=(bin[i]-'0')*count;
        count*=2;
    }
    return dec;
}
//10进制转换2进制
char* decimal_convert_binary(unsigned int dec)
{
    int len=8;
    char *bin = (char*)malloc(len+1); //  
    if (bin == NULL) {  
        return NULL;  
    }  
    for(int i=0;i<len;++i)
    {
        bin[i]='0';
    }
    bin[len]='\0';

    int i = len - 1;   
    while (dec > 0) {  
        bin[i--] = (dec % 2) + '0';  
        dec /= 2;  
    }  
     
    return bin;  
}
//设置两个pin短路,设置pin1和pin2数据一致
void set_short()
{
    printf("请输入模拟短路的Pin number:\n");
    scanf("%d %d",&pin_1,&pin_2);
    printf("将pin:%d和pin:%dshort!\n",pin_1,pin_2);
}
//对某地址写入8bit数据
uint8_t write_to_address(const char *address_str, char *bin)
{
    unsigned int address=binary_convert_decimal(address_str);
    
    printf("write to address:0x%X,%s\n",address,bin);
    
    //将pin_1和pin_2对应bit位一致
    bin[8-1-pin_2]=bin[8-1-pin_1];
   
    uint8_t data=binary_convert_decimal(bin);
    sram[address]=data;

    return data;
}
//对某地址读取8bit数据
void read_to_address(const char *address_str)
{
    unsigned int address=binary_convert_decimal(address_str);
    uint8_t result=sram[address];
    char *data=decimal_convert_binary(result);
    
    printf("read to address:0x%X,%s\n",address,data);
}
//对比写入和读取的数据，看pin_1和pin_2的bit位是否一致，
void check_data(const char *address_str,const char *write_data)
{
    
    unsigned int address=binary_convert_decimal(address_str);
    uint8_t result=sram[address];
    char *data=decimal_convert_binary(result);

    uint8_t w_data=binary_convert_decimal(write_data);
    
    
    if(result != w_data)
    {
        printf("输入与读取数据不一致：\n");
        if(data[7-pin_1]==data[7-pin_2])
        {
            printf("证实%d和%d短路!\n",pin_1,pin_2);
        }else{
            printf("非%d和%d短路造成!\n",pin_1,pin_2);
        }
    }else{
        printf("输入和读取数据一致!\n");
    }

}


int main()
{
    InitPinTypes();
    char address[16];
    char data[9];
    char write_data[9];

    printf("请输入address和data:\n");
    while(1)
    {
        scanf("%s %s",address,data);
        strcpy(write_data,data);
        printf("%s %s\n",address,data);

        set_short(pin_1,pin_2);
        if(check_pin_type(pin_1,pin_2)==0)
        {
            printf("模拟的两个pin类型检查不通过!\n");
            continue;
        }

        write_to_address(address,data);
        read_to_address(address);

        check_data(address,write_data);
    }
    return 0;
}