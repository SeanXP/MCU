/***************************************************
 *         Author: Shawn Guo                       *
 *         Date  : 2013/2/12                       *
 *         Last  : 2013/2/12                       *
 *         Notes : TIMER                           *
 *          P0 | JP12(0 1 ... H G)                 *
 **************************************************/

#include <reg52.h>

// 74HC138        �����λѡ����
sbit HC138A = P2^2;
sbit HC138B = P2^3;
sbit HC138C = P2^4;

// Nixie Tube(-)     �͵�ƽλѡ����ܣ� �ߵ�ƽ��ѡ�������� ��������ȡ��Tube(-) ����ȡ��
// a b ... g dp (����ܶ�ѡ�ӿ�)
// 8 7 ... 2 1  (��Ӧ��ƽ�ӿ�)
//                                  0      1       2      3     4      5      6      7       8     9      A      b      C      d      E      F
unsigned char code NUM_TABLE[] = {~0x03, ~0x9f, ~0x25, ~0x0d, ~0x99, ~0x49, ~0x41, ~0x1f, ~0x01, ~0x09, ~0x11, ~0xc1, ~0x63, ~0x85, ~0x61, ~0x71};
unsigned int COUNT0, COUNT1;  //��ʱ���ж���Ӧ������

void Delayms(unsigned int xms);
void Time_Init(); //��ʱ����ʼ����


int main()
{
    unsigned char i = 0;
    unsigned char led[8];

    Time_Init();

    while(1)
    {
        //����ܶ�ѡ����
        led[0] = NUM_TABLE[COUNT0 % 10000 / 1000]; //ǧλ����
        led[1] = NUM_TABLE[COUNT0 % 1000 / 100]; //��λ����
        led[2] = NUM_TABLE[COUNT0 % 100 / 10]; //ʮλ����
        led[3] = NUM_TABLE[COUNT0 % 10]; //��λ����

        led[4] = NUM_TABLE[COUNT1 % 10000 / 1000]; //ǧλ����
        led[5] = NUM_TABLE[COUNT1 % 1000 / 100]; //��λ����
        led[6] = NUM_TABLE[COUNT1 % 100 / 10]; //ʮλ����
        led[7] = NUM_TABLE[COUNT1 % 10]; //��λ����

        //�����λѡ����
        for(i = 0; i < 8; i++)
        {
            P0 = led[i];//��ѡ�ź�
            switch(i) // λѡ�ź�
            {
                case 0: HC138C = 0; HC138B = 0; HC138A = 0; break;
                case 1: HC138C = 0; HC138B = 0; HC138A = 1; break;
                case 2: HC138C = 0; HC138B = 1; HC138A = 0; break;
                case 3: HC138C = 0; HC138B = 1; HC138A = 1; break;
                case 4: HC138C = 1; HC138B = 0; HC138A = 0; break;
                case 5: HC138C = 1; HC138B = 0; HC138A = 1; break;
                case 6: HC138C = 1; HC138B = 1; HC138A = 0; break;
                case 7: HC138C = 1; HC138B = 1; HC138A = 1; break;
            }
            Delayms(2);
        }

    }

    return 0;
}

void Time_Init() //��ʱ����ʼ����
{
    //��ʱ��������ʽ����
    //             TIME1           TIME0
    // TMOD = GATE C/T M1 M0   GATE C/T M1 M0
    TMOD = 0x00; //ѡ��ģʽ0  13λ���� ������8192

    //Ԥװ��ֵ
    TH1 = TH0 = (8192 - 1000) / 32;
    TL1 = TL0 = (8192 - 1000) % 32;

    //�ж�����Ĵ��� IE���� EA - ET2 ES ET1 EX1 ET0 EX0
    IE = 0x8A;   // EA = 1, ET1 = 1, ET0 = 1
    TR0 = 1;    //����T0��ʱ��
    TR1 = 1;
}

void Delayms(unsigned int xms)      // ���뼶��ʱ
{
    unsigned char i;

    while(xms--)            // --xms �� xms-- �ڻ�����в��,����С����һЩDelayms(1)����䣬��˻��������xms--
        for(i = 0; i < 110; i++)
            ;
}

//��ʱ��0 �ж���Ӧ����
void Timer0() interrupt 1
{
    static unsigned char tem = 0;
    TH0 = (8192 - 1000) / 32;
    TL0 = (8192 - 1000) % 32;
    if(++tem >= 100){
        tem = 0;
        COUNT0++;
    }
}

//��ʱ��1 �ж���Ӧ����
void Timer1() interrupt 3
{
    static unsigned char te = 0;
    TH1 = (8192 - 1000) / 32;
    TL1 = (8192 - 1000) % 32;
    if(++te >= 200){
        te = 0;
        COUNT1++;
    }
}
