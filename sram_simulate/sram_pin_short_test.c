#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define SRAM_SIZE (32*1024)
#define ADDRESS_PIN 15
#define IO_PINS 8
#define CONTROL_PINS 3
#define TOTAL_PINS (ADDRESS_PIN+IO_PINS+CONTROL_PINS+2)//包括VCC,GND

//sram的缓冲区
uint8_t sram[SRAM_SIZE];

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

//引脚状态
typedef enum{
    PIN_STATE_LOW=0,
    PIN_STATE_HIGH=1
}PinState;

//引脚类型表
PinType pinTypes[TOTAL_PINS];
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

//对某地址写8bit数据
void write_to_address(int address,uint8_t data)
{
    if(address >= SRAM_SIZE)
    {
        printf("Error:Address out of range!\n");
        return;
    }
    sram[address]=data;
    printf("Data written to address 0x%X:0x%X\n",address,data);
}
//从该地址读取8bit数据
unsigned char read_to_address(int address)
{
    if(address >= SRAM_SIZE)
    {
        printf("Error:Address out of range!\n");
        return 0xFF;//错误码
    }
    printf("Data read to address 0x%X:0x%X\n",address,sram[address]);
    return sram[address];
}
//检查引脚类型
int checkPinSame(int pin1,int pin2)
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
PinState pin_states[TOTAL_PINS]={PIN_STATE_LOW};//存储每个引脚状态

//设置pin的状态
void write_pin(int pin_index,PinState state)
{
    
        pin_states[pin_index-1]=state;
}
//读取pin状态
PinState read_pin(int pin_index)
{
    return pin_states[pin_index-1];
}
//检测两个pin连在一块，
int check_pins_connected(int pin1,int pin2)
{
    
    write_pin(pin1-1,PIN_STATE_HIGH);
    write_pin(pin2-1,PIN_STATE_LOW);

    printf("%d %d\n",read_pin(pin1-1),read_pin(pin2-1));
    if(read_pin(pin1-1)==read_pin(pin2-1))//pin1与pin2可能焊接在一块
    { 
        return 1;
    }
    return 0;
    
}


int main()
{
    InitPinTypes();//初始化引脚数组

    write_to_address(1024,0xAA);
    sram[1024]=0xAB;
    char read_data=read_to_address(1024);
    int pin1,pin2;
    
    if(read_data!=0xAA)//读写不一致，可能发生引脚short错误
    {  
        printf("Pin detection:\n");
        while(scanf("%d%d",&pin1,&pin2)!=EOF)
        {
            printf("检测中\n");
            if(0==checkPinSame(pin1,pin2))
            {
                printf("pin1,pin2类型有误或不相邻,跳出检测\n");
                continue;
            }
            //检测哪两个pin连在一块
            if(check_pins_connected(pin1,pin2))
            {
                printf("pin1与pin2可能焊接在一块!\n");
                return 0;
            }else{
                printf("These two pins are correct!\n");
            }
        }
        printf("检测完毕!\n");
    }
    return 0;
}


