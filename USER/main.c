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
    float temp1;     //温度数据
    float humid1;    //湿度数据
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置NVIC中断分组2
    usart_init(115200);    //串口初始化波特率
    LED_Init();       //LED端口初始化
    AM2320_Init();    //初始化温湿度IIC
    IIC_Init1();    //初始化光照IIC
    GY_30_Init();  //初始化GY-30
    delay_ms(100);




    while (1)
    {

        GY_30_SendMode(0x01);  //上电
        GY_30_SendMode(0x10);  //设置为连续高分辨率模式
        delay_ms(180);  //最多延时180ms

        //唤醒传感器并发送读指令
        AM2320_Wakeup();
        AM2320_Send_Read();
        delay_ms(3);

        //将读取的数据转换为光强
        light = GY_30_Read();
        light1 = light / 1.2;

        //将读取的数据转换为百进制温湿度
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

        printf("\r\n当前温度为：%g ℃\r\n", temp1);
        printf("\r\n当前湿度为：%g %%RH\r\n", humid1);
        printf("\r\n当前光强为：%g lx\r\n\r\n", light1);

        delay_ms(1000);
        delay_ms(1000);
    }

}
