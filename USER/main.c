#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "myiic.h"
#include "myiic1.h"
#include "AM2320.h"
#include "GY-30.h"


int main(void)
{
//    u16 t;
//    u16 len;
    u16 times = 0;
    u16 temp;
    u16 humid;
    u32  data;
    u16 light;
    float light1;
    float temp1;     //�¶�����
    float humid1;    //ʪ������
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //����NVIC�жϷ���2
    usart_init(115200);    //���ڳ�ʼ��������
    LED_Init();       //LED�˿ڳ�ʼ��
    AM2320_Init();    //��ʼ����ʪ��IIC
    IIC_Init1();    //��ʼ������IIC
    GY_30_Init();  //��ʼ��GY-30
    delay_ms(100);




    while (1)
    {

        GY_30_SendMode(0x01);  //�ϵ�
        GY_30_SendMode(0x10);  //����Ϊ�����߷ֱ���ģʽ
        delay_ms(180);  //�����ʱ180ms

        //���Ѵ����������Ͷ�ָ��
        AM2320_Wakeup();
        AM2320_Send_Read();
        delay_ms(3);

        //����ȡ������ת��Ϊ��ǿ
        light = GY_30_Read();
        light1 = light / 1.2;

        //����ȡ������ת��Ϊ�ٽ�����ʪ��
        data = AM2320_Read() ;
        humid = data >> 16;
        humid1 = humid / 10.0; //((humid / 256) * 256 + ((humid % 256) / 16) * 16 + ((humid % 256) % 16)) * 1.0 / 10;
        temp = data;
        temp1 = temp / 10.0; //(temp / 256) * 256 + ((temp % 256) / 16) * 16 + ((temp % 256) % 16)) * 1.0 / 10;

        times++;
        if (times % 1 == 0)
        {
            LED0 = !LED0;
            times = 0;
        }

        printf("\r\n��ǰ�¶�Ϊ��%g ��\r\n", temp1);
        printf("\r\n��ǰʪ��Ϊ��%g %%RH\r\n", humid1);
        printf("\r\n��ǰ��ǿΪ��%g lx\r\n\r\n", light1);

        delay_ms(1000);
        delay_ms(1000);
    }

}
