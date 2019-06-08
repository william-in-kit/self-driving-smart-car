/**************************************************************
** Copyright(c) 2011~2012 Smartcar Lab of USTB
** All Rights Reserve
**-------------------------------------------------------------
** MCU         : k60
** Crystal     :
** Module Name : sensorcollect.c
** Author      : Haoyu Tang
** Date        : 2014.09.21
** Version     : v1.0
** description :
** Notes       :
**-------------------------------------------------------------
** Reversion    :
** Modified By  :
** Modified Date:
** Notes        :
**************************************************************/

#include "common.h"
#include "schedule.h"

/******************************************************************************/

/*--------------- V A R I A B L E S ------------------*/
//CCD变量
uint8 middle = 0;

//速控变量

int16 g_motor_set = 80;  //预设速度，用于PID调节

int16 g_motor_set_slowdown1 = 30;  //在直道高速时探测到小角度变化时减速用的值
int16 g_motor_set_slowdown2 = 20;
int16 g_motor_set_turn = 40;     //弯道时看到大角度变化时用的速度
//小角度检测用的阈值
int angthreshold1 = 2;
int angthreshold2 = 4;

//g_motor_set_turn = g_motor_set;


int g_speedvalue_temp = 0;      //保存码盘反馈顺时值
int16 g_speed_feedback = 0;      //速度采集
signed int PWM_motor_value = 0;      //电机PWM值

//舵机变量
int16 g_servo_set = PWM_SVO_MIDDLE;
unsigned int PWM_turn_value = 0;     //转向舵机PWM值

float turn_p = 37;
float turn_d = 3.5;

//上位机调试标记
int g_ccd_sendflag = 0;
//角度误差检测标记
int angerrorflag = 0; //0为 角度变化在-1至1 之间， 1表示2至4或-2至-4之间  2表示大于5或小于-5

//计时
uint8 time=0;
/*--------------- F U N C T I O N S ----------------------------------*/

/**************************************************************
Function Name: pit0_isr
Date         : 2014.02.18
Description  :
Input		 : None
Output		 : None
Notes		 : 定时器0中断服务函数
**************************************************************/
void pit0_isr(void)
{
    //clear the interrupt flag to avoid re-entrance
    CLR_FLAG_PIT0();

    //计时重置
    TIME0_RESET();

    //CCD采集
    CCD_Collect(&ccd0);
    middle = CCD_Identify(&ccd0);

    //速度采集
    PULSE_COUNTER_GET(g_speedvalue_temp);       //读取码盘反馈值
    g_speed_feedback = - g_speedvalue_temp;
    
    g_angerror_temp = (middle - g_ccd_middle);  //角度误差计算（从原来的Car_Turn()提出来了  201410131946
    
    if(g_angerror_temp >= angthreshold1 && g_angerror_temp <= angthreshold2)
    {
       angerrorflag = 1;
    }
    if(g_angerror_temp >= -angthreshold2 && g_angerror_temp <= -angthreshold1)
    {
       angerrorflag = 1;
    }
    if(g_angerror_temp > angthreshold2 && g_angerror_temp < -angthreshold2)
    {
       angerrorflag = 2;
    }
    if(g_angerror_temp < angthreshold1 && g_angerror_temp > -angthreshold1)
    {
       angerrorflag = 0;
    }
    
    //速度控制
    //Car_Run();

    //转向控制
    Car_Turn();

    //CCD调试
    g_ccd_sendflag = 1;
}

/*
function name:  car_run
parameters:     none
author:         Haoyu Tang
date:           2014-9-21
description:    control car run forward
*/
void Car_Run(void)
{
     
        motorPID.vi_FeedBack = g_speed_feedback;   //反馈值采样
        //g_speed_feedback = 0; 
        
        
        //速度控制方法1，入弯减速出弯加速。更细一点说，在直道 高速 时若探测到小角度变化则大减速。在弯道观察到大角度变化则中速，出弯高速。    
        //高速状态下若无角度变化则使用高速80，若遇到小角度变化则减速至30，若有大角度变化则减速至20。
        
        //if(g_speed_feedback > 70)//此值需要实测。
        //{
        //    switch(angerrorflag)
        //    {
        //        case 0: motorPID.vi_Ref = g_motor_set;break;
        //        case 1: motorPID.vi_Ref =  g_motor_set_slowdown1;break;
        //        case 2: motorPID.vi_Ref = g_motor_set_slowdown2;break;
        //        default: motorPID.vi_Ref = g_motor_set;
       //     }           
       // }
        //中速段若无角度变化则使用高速80，若小角度则使用 60；若大角度则使用 40；
        //else if(g_speed_feedback > 40 && g_speed_feedback <=70)
        //{
        //    switch(angerrorflag)
        //    {
        //        case 0: motorPID.vi_Ref = g_motor_set;break;
        //        case 1: motorPID.vi_Ref = g_motor_set-10;break;
        //        case 2: motorPID.vi_Ref = g_motor_set_turn;break;
        //        default: motorPID.vi_Ref = g_motor_set;
        //    }
        //}
        //低俗阶段若无角度变化则使用高速80，若小角度变化则使用 60，若大角度则使用40
        //else
        //{
        //    switch(angerrorflag)
         //    {
        //        case 0:motorPID.vi_Ref = g_motor_set;break;
        //        case 1:motorPID.vi_Ref = g_motor_set_turn + 10;break;
        //        case 2:motorPID.vi_Ref = g_motor_set_turn;break;
        //        default:motorPID.vi_Ref = g_motor_set;
       //      }
       // }
        
        
        //速度控制方法2：找到速度与角度误差之间的关系。参照学长摄像头车的速度控制方法       
        //原学长原公式:sPID.vi_Ref=g_HighestSpeed- (59-g_Control)*(59-g_Control)*(g_Highest - g_LowestSpeed)/3481
       //motorPID.viRef = g_motor_set - (angerror_temp)*(angerror_temp)*(g_motor_set - g_motor_turn)/3481;
        
        
        //第一次分站赛算法，仅仅是入弯减速出弯加速。
        if(PWM_turn_value>= 4800||PWM_turn_value<=4600)
        {
             motorPID.vi_Ref = g_motor_set_turn;//若转弯过大则调用转弯时较小的的电机预设值
        }
        else
        {
           motorPID.vi_Ref = g_motor_set;  //正常直道
        }
        
        
        
        g_speed_feedback = 0; 
        //motorPID.vi_Ref = g_motor_set;
        
          
        PWM_motor_value = v_PIDCalc(&motorPID);         //调用电机PID          

        if(PWM_motor_value > 1000)
        {
            PWM_motor_value = 1000;
        }
        else if(PWM_motor_value < -1000)
        {
            PWM_motor_value = -1000;
        }

        SET_PWM_MOT(PWM_motor_value);
    
}

/*
function name:  car_turn
parameters:     none
author:         Haoyu Tang
date:           2014-9-21
description:    control car turn
*/
void Car_Turn(void)
{
    //中值转换为偏离量，方便计算舵机转向
    //g_angerror_temp = (middle - g_ccd_middle);（已写到中断函数中）
    
    
    /* 方法1：动态kp  参考学长摄像头车的控制方法 */    
    /*  turn_p =  a*(g_angerror_temp*g_angerror_temp)+b;    具体a和b需要上车调。  */
    
    
    //方法2，在小角度g_angerror_temp时用一个小的定kp，这样可以使小车在小s时接近直线通过，在大g_angerror_temp时
    //再用动态kp方法或者直接给个大的kp   
    /*
    if(g_angerror_temp <= 3 && g_angerror_temp >= -3)
    {
        turn_p= 15;  //这个值需要在连续小s弯那里调到使车子直线通过;
     }
    else
    {
        turn_p = a*(g_angerror_temp*g_angerror_temp)+ b; 
    }
    */
  
    //方法3 ，直接加上入弯判断标志位，当入弯时，将车子的行驶轨迹从中线偏到内弯一侧
    /*
    if(g_angerror_temp < 4)
    {   
       g_angerror_temp = middle - 15 - g_ccd_middle;
    }
    if(g_angerror_temp > 4)
    {
      g_angerror_temp = middle + 15 + g_ccd_middle;
    }
    */
       

    PWM_turn_value = (int)(PWM_SVO_MIDDLE - (g_angerror_temp * turn_p) - (g_angerror_temp - last_g_angerror_temp) * turn_d);
    last_g_angerror_temp = g_angerror_temp;

    if (PWM_turn_value <= PWM_SVO_MIN)
    {
        PWM_turn_value = PWM_SVO_MIN;
    }
    else if (PWM_turn_value >= PWM_SVO_MAX)
    {
        PWM_turn_value = PWM_SVO_MAX;
    }

    //经过非门
    SET_PWM_SVO(PWM_turn_value);

}
//ccd调试
/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：SendImageData
*  功能说明：
*  参数说明：
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*************************************************************************/
void SendImageData(char ph[])
{
    unsigned char ccd_data[128];
    unsigned char i;
    unsigned char crc = 0;

    /* Send Data */
    uart_putchar('*');
    uart_putchar('L');
    uart_putchar('D');

    SendHex(0);
    SendHex(0);
    SendHex(0);
    SendHex(0);
    for(i=0; i<128; i++)
    {
      ccd_data[i] = ph[i];
    }
    for(i=0; i<128; i++) {
      SendHex( ccd_data[i]);
    }

    SendHex(crc);
    uart_putchar('#');
}



/*************************************************************************
*                           蓝宙电子工作室
*
*  函数名称：SendHex
*  功能说明：采集发数程序
*  参数说明：
*  函数返回：无
*  修改时间：2012-10-20
*  备    注：
*************************************************************************/
void SendHex(unsigned char hex)
{
    unsigned char temp;
    temp = hex >> 4;
    if(temp < 10) {
      uart_putchar(temp + '0');
    } else {
      uart_putchar(temp - 10 + 'A');
    }
    temp = hex & 0x0F;
    if(temp < 10) {
      uart_putchar(temp + '0');
    } else {
     uart_putchar(temp - 10 + 'A');
    }
}