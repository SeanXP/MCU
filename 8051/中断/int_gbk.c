/***************************************************
 *         Author: Shawn Guo                       *
 *         Date  : 2013/2/12                       *
 *         Last  : 2013/2/12                       *
 *         Notes : INT0 + INT1                     *
 *          P0 | JP12(0 1 ... H G)                 *
 *          P3 | JP5 (0 1 ... A B)                 *
 *            J1��·ñ�Ͽ����������ң���ж�Ӱ��  *
 *  һ���ܼ�ª���ⲿ�жϼ�����������û�з�������  *
 *    ����: ����������ܣ�����ʵ���ⲿ�жϼ�����  *
 **************************************************/

#include <reg52.h>
#include <intrins.h>   // _nop_();

// 74HC138        �����λѡ����
sbit HC138A = P2^2;
sbit HC138B = P2^3;
sbit HC138C = P2^4;

// Nixie Tube(-)     �͵�ƽλѡ����ܣ� �ߵ�ƽ��ѡ�������� ��������ȡ��Tube(-) ����ȡ��
// a b ... g dp (����ܶ�ѡ�ӿ�)
// 8 7 ... 2 1  (��Ӧ��ƽ�ӿ�)
//                                  0      1       2      3     4      5      6      7       8     9      A      b      C      d      E      F
unsigned char code NUM_TABLE[] = {~0x03, ~0x9f, ~0x25, ~0x0d, ~0x99, ~0x49, ~0x41, ~0x1f, ~0x01, ~0x09, ~0x11, ~0xc1, ~0x63, ~0x85, ~0x61, ~0x71};
unsigned int COUNT0 = 0, COUNT1 = 0; //�ж���Ӧ������

void Delayms(unsigned int xms);


int main()
{
    unsigned char i = 0;
    unsigned char led[8];

    P0 = 0xff;  // ��ʼ����(��ɲ��أ�һ���޹�λ��Ƭ���ӵ���Զ��øߵ�ƽ)
    P1 = 0xff;
    P2 = 0xff;

    //�ⲿ�ж�0 ����
    //�жϴ�����ʽ���� IT0 = 0���͵�ƽ������ʽ
    IT0=1;       //IT0 = 1���½��ش���
    EX0=1; //�����ⲿ�ж�0 �ж�����



    //�ⲿ�ж�1 ����
    //�жϴ�����ʽ���� IT1 = 0���͵�ƽ������ʽ
    IT1=1;        //IT1 = 1���½��ش���
    EX1=1; //�����ⲿ�ж�1 �ж�����

    //����ȫ���ж�����λ(�жϻ���ǰ��)
    EA=1;

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

void Delayms(unsigned int xms)      // ���뼶��ʱ
{
    unsigned char i;

    while(--xms)            // --xms �� xms-- �ڻ�����в��
        for(i = 0; i < 110; i++)
            ;
}

// �ⲿ�ж�0
void  Counter0() interrupt 0  using 1 //ʹ�õ�Ƭ���ڴ�4�鹤���Ĵ����е�1��
{

   EX0 = 0;  // ��ʱ�����ⲿ�ж�0

   COUNT0++;  //�ж���Ӧ������

   EX0 = 1;  // �������
}

//�ⲿ�ж�1
void  Counter1() interrupt 2 using 2//ʹ�õ�Ƭ���ڴ�4�鹤���Ĵ����е�2��
{

   EX1 = 0; // ��ʱ�����ⲿ�ж�1

   COUNT1++;//�ж���Ӧ������

   EX1 = 1;// �������
}
