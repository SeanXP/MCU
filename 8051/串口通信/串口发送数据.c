/***************************************************
 *         Author: Shawn Guo                       *
 *         Date  : 2013/2/13                       *
 *         Last  : 2013/2/13                       *
 *         Notes : PL2303串口通信                  *
 *         串口调试助手sscom32                     *
 *      Baud 4800、数据位8、停止位1、效验位无     *
 **************************************************/

#include<reg52.h>

unsigned char code MESSAGE[] = "This is Shawn!";

void Delayms(unsigned int xms);
void Init();     // 串口通信初始化

int main()
{
    unsigned char i = 0;

    Init();

    while(1)           //循环 发送数据
    {
        i = 0;

        while(MESSAGE[i] != '\0')
        {
            SBUF = MESSAGE[i];//SUBF接受/发送缓冲器(又叫串行通信特殊功能寄存器)
            while(!TI); //  等待数据传输完成(传送完成后产生TI中断)

            TI = 0;	// 清除发送数据TI中断标志
            i++;// 准备发送下一个字符
        }
        Delayms(2000);
    }

    return 0;
}

void Init()     // 串口通信初始化
{
    // 串行口控制寄存器 SCON
    // SM0 SM1         SM2             REN            TB8           RB8       TI             RI
    // 工作方式选择位  多机通信控制位  允许串行接受位 发送数据第9位 接受第9位 发送中断标志位 接受中断标志位
    SCON = 0x50;// 0101 0000, 0 1 串口工作模式1, REN=1允许串行接受
    //串行口工作方式1(10位数据，低电平起始位， 8位数据，高电平停止位)


    //定时器工作方式配置
    //             TIME1           TIME0
    // TMOD = GATE C/T M1 M0   GATE C/T M1 M0
    TMOD |= 0x20; // 0010 0000,定时器1 工作方式2,8位初值自动重装，精确波特率


    //电源管理寄存器 PCON
    // SMOD                 SMOD0 LVDF P0F  GF1 GF0            PD        IDL
    // 串口通信波特率调节                   通用工作标志位     掉电模式  空闲模式
    PCON |= 0x80; //波特率提高一倍


    //由定时器1溢出频率来设定串口通信波特率
    // TH1  = 0xFD;     //baud*2  /* reload value 19200、数据位8、停止位1。效验位无(11.0592)
    TH1 = 0xF3;	//baud*2  /*  波特率4800、数据位8、停止位1。效验位无 (12M)
    TL1 = 0xF3;

    TR1  = 1;        //开启定时器1
    ES   = 1;        //开串口中断允许
    EA   = 1;        // 开总中断允许
}

void Delayms(unsigned int xms)
{
    unsigned char i;

    while(--xms)
    {
        for(i = 0; i < 110; i++)
            ;
    }
}
