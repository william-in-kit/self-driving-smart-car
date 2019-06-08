/**************************************************************
** Copyright(c) 2014 Smartcar Lab of USTB
** All Rights Reserve
**-------------------------------------------------------------
** MCU         : K60(Freescale)
** Crystal     :
** Module Name : main
Author         : Haoyu Tang
Date           : 2014.09.22
** Version     : 例程。唐浩宇
** description :
** Notes       :
**-------------------------------------------------------------
** Reversion    :
** Modified By  :
** Modified Date:
** Notes        :
**************************************************************/

#include "common.h"

/*--------------------M A C R O S--------------------*/

/*-----------------V A R I A B L E S-----------------*/

int a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;

void main(void)
{
    
    DisableInterrupts;  //程序一开始需要关闭所有中断，防止单片机初始化过程中被某个中断打断
    system_init(); //系统初始化    
    
    
    a = SWITCH1_STATUS;
    b = SWITCH2_STATUS;
    c = SWITCH3_STATUS;
    d = SWITCH4_STATUS;
    //e = 8*a + 4*b + 2*c + d;
    /*
    switch(e)
    {
    case 0 : g_motor_set=10;break;                   //保底：开环 ，电机p波给400
    case 1 : g_motor_set=speed_1;break;            //速度1： 闭环，直道48，弯道39，亲测稳定可跑。
    case 2 : g_motor_set=speed_2;break;
    case 3 : g_motor_set=speed_3;break;
    case 4 : g_motor_set=speed_4;break;
    default : g_motor_set=speed_1;
    }*/
    
    if(a==1&&b==0&&c==0&&d==0)
    {
      g_motor_set=42;
      g_motor_set_turn=37;
      //g_speed_p=250;
      //g_speed_i=13;
      //g_speed_d=0;
    }
    else if(a==0&&b==1&&c==0&&d==0)
    {
      g_motor_set=48;
      g_motor_set_turn=37;
      //g_speed_p=250;
      //g_speed_i=13;
      //g_speed_d=0;
    }
    else if(a==0&&b==0&&c==1&&d==0)
    {
      g_motor_set=55;
      g_motor_set_turn=40;
      //g_speed_p=250;
      //g_speed_i=13;
      //g_speed_d=0;
    }
    else if(a==0&&b==0&&c==0&&d==1)
    {
      g_motor_set=57;
      g_motor_set_turn=40;
      //g_speed_p=250;
      //g_speed_i=13;
      //g_speed_d=0;
    }
    else
    {
       g_motor_set=38;
       g_motor_set_turn=38;
       //g_speed_p=250;
       //g_speed_i=13;
       //g_speed_d=0;
    }
    
    Delay_ms(2500);
   
    EnableInterrupts;   //开中断，定时器中断开始工作


    /*-----------------程序进行正确循环阶段-----------------*/

    for( ; ; )
    {
        SET_PWM_MOT(350);
        if(g_ccd_sendflag==1)
        {
            g_ccd_sendflag = 0;
            SendImageData(ccd0.graph);
        }
    }
}