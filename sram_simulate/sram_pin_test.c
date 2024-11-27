#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SRAM_SIZE (32*1024)
#define MAX_PINS 28

//sram的缓冲区
unsigned char sram[SRAM_SIZE];

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

//引脚类型表
PinType pinTypes[MAX_PINS];

//初始化引脚类型
void initPinTypes()
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
    if(pinTypes[pin1]!=pinTypes[pin2])
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
            if(abs(pin1,pin2)>1)//pin1 ，pin2间距大于1
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
            if(abs(pin1,pin2)>1)//pin1 ，pin2间距大于1
            {
                return 0;
            }
        }
    }
}

//写入数据
void write_sram(int address,unsigned char data)
{
    if(address >= SRAM_SIZE)
    {
        printf("Error:Address out of range!\n");
        return;
    }
    sram[address]=data;
    printf("Data written to address 0x%X:0x%X\n",address,data);
}

//读取数据
unsigned char read_sram(int address)
{
    if(address >= SRAM_SIZE)
    {
        printf("Error:Address out of range!\n");
        return 0xFF;//错误码
    }
    return sram[address];
}

int main(int argc,char *argv[])
{
    if(argc !=3)
    {
        printf("Usage:%s pin1 pin2\n",argv[0]);
        return -1;
    }

    initPinTypes();//初始化引脚类型

    int pin1=argv[1]-'0';
    int pin2=argv[2]-'0';

    if(checkPinSame(pin1,pin2))
    {
        int address1=
        char write_data1='0xAA';
        char write_data2='0xAB';
        char read_data1,read_data2;

        write_sram()
    }
}



