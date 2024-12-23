#include <stdio.h>
#include <stdint.h>

#define DATA_SIZE 1


//模拟内部存储
uint8_t internalStorage=0;

void write_to_ic(uint8_t data)
{
    internalStorage = data;
    printf("Date write to ic:0x%X\n",data);
}

uint8_t read_from_ic(void)
{
    return internalStorage;
}

int main()
{
    uint8_t data_write=0xAB;
    uint8_t data_read;

    write_to_ic(data_write);
    data_read=read_from_ic();

    printf("Date read from IC:0x%X\n",data_read);
    return 0;
}