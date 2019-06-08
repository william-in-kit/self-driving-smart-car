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
//CCD����
uint8 middle = 0;

//�ٿر���

int16 g_motor_set = 80;  //Ԥ���ٶȣ�����PID����

int16 g_motor_set_slowdown1 = 30;  //��ֱ������ʱ̽�⵽С�Ƕȱ仯ʱ�����õ�ֵ
int16 g_motor_set_slowdown2 = 20;
int16 g_motor_set_turn = 40;     //���ʱ������Ƕȱ仯ʱ�õ��ٶ�
//С�Ƕȼ���õ���ֵ
int angthreshold1 = 2;
int angthreshold2 = 4;

//g_motor_set_turn = g_motor_set;


int g_speedvalue_temp = 0;      //�������̷���˳ʱֵ
int16 g_speed_feedback = 0;      //�ٶȲɼ�
signed int PWM_motor_value = 0;      //���PWMֵ

//�������
int16 g_servo_set = PWM_SVO_MIDDLE;
unsigned int PWM_turn_value = 0;     //ת����PWMֵ

float turn_p = 37;
float turn_d = 3.5;

//��λ�����Ա��
int g_ccd_sendflag = 0;
//�Ƕ��������
int angerrorflag = 0; //0Ϊ �Ƕȱ仯��-1��1 ֮�䣬 1��ʾ2��4��-2��-4֮��  2��ʾ����5��С��-5

//��ʱ
uint8 time=0;
/*--------------- F U N C T I O N S ----------------------------------*/

/**************************************************************
Function Name: pit0_isr
Date         : 2014.02.18
Description  :
Input		 : None
Output		 : None
Notes		 : ��ʱ��0�жϷ�����
**************************************************************/
void pit0_isr(void)
{
    //clear the interrupt flag to avoid re-entrance
    CLR_FLAG_PIT0();

    //��ʱ����
    TIME0_RESET();

    //CCD�ɼ�
    CCD_Collect(&ccd0);
    middle = CCD_Identify(&ccd0);

    //�ٶȲɼ�
    PULSE_COUNTER_GET(g_speedvalue_temp);       //��ȡ���̷���ֵ
    g_speed_feedback = - g_speedvalue_temp;
    
    g_angerror_temp = (middle - g_ccd_middle);  //�Ƕ������㣨��ԭ����Car_Turn()�������  201410131946
    
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
    
    //�ٶȿ���
    //Car_Run();

    //ת�����
    Car_Turn();

    //CCD����
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
     
        motorPID.vi_FeedBack = g_speed_feedback;   //����ֵ����
        //g_speed_feedback = 0; 
        
        
        //�ٶȿ��Ʒ���1��������ٳ�����١���ϸһ��˵����ֱ�� ���� ʱ��̽�⵽С�Ƕȱ仯�����١�������۲쵽��Ƕȱ仯�����٣�������١�    
        //����״̬�����޽Ƕȱ仯��ʹ�ø���80��������С�Ƕȱ仯�������30�����д�Ƕȱ仯�������20��
        
        //if(g_speed_feedback > 70)//��ֵ��Ҫʵ�⡣
        //{
        //    switch(angerrorflag)
        //    {
        //        case 0: motorPID.vi_Ref = g_motor_set;break;
        //        case 1: motorPID.vi_Ref =  g_motor_set_slowdown1;break;
        //        case 2: motorPID.vi_Ref = g_motor_set_slowdown2;break;
        //        default: motorPID.vi_Ref = g_motor_set;
       //     }           
       // }
        //���ٶ����޽Ƕȱ仯��ʹ�ø���80����С�Ƕ���ʹ�� 60������Ƕ���ʹ�� 40��
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
        //���׽׶����޽Ƕȱ仯��ʹ�ø���80����С�Ƕȱ仯��ʹ�� 60������Ƕ���ʹ��40
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
        
        
        //�ٶȿ��Ʒ���2���ҵ��ٶ���Ƕ����֮��Ĺ�ϵ������ѧ������ͷ�����ٶȿ��Ʒ���       
        //ԭѧ��ԭ��ʽ:sPID.vi_Ref=g_HighestSpeed- (59-g_Control)*(59-g_Control)*(g_Highest - g_LowestSpeed)/3481
       //motorPID.viRef = g_motor_set - (angerror_temp)*(angerror_temp)*(g_motor_set - g_motor_turn)/3481;
        
        
        //��һ�η�վ���㷨��������������ٳ�����١�
        if(PWM_turn_value>= 4800||PWM_turn_value<=4600)
        {
             motorPID.vi_Ref = g_motor_set_turn;//��ת����������ת��ʱ��С�ĵĵ��Ԥ��ֵ
        }
        else
        {
           motorPID.vi_Ref = g_motor_set;  //����ֱ��
        }
        
        
        
        g_speed_feedback = 0; 
        //motorPID.vi_Ref = g_motor_set;
        
          
        PWM_motor_value = v_PIDCalc(&motorPID);         //���õ��PID          

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
    //��ֵת��Ϊƫ���������������ת��
    //g_angerror_temp = (middle - g_ccd_middle);����д���жϺ����У�
    
    
    /* ����1����̬kp  �ο�ѧ������ͷ���Ŀ��Ʒ��� */    
    /*  turn_p =  a*(g_angerror_temp*g_angerror_temp)+b;    ����a��b��Ҫ�ϳ�����  */
    
    
    //����2����С�Ƕ�g_angerror_tempʱ��һ��С�Ķ�kp����������ʹС����Сsʱ�ӽ�ֱ��ͨ�����ڴ�g_angerror_tempʱ
    //���ö�̬kp��������ֱ�Ӹ������kp   
    /*
    if(g_angerror_temp <= 3 && g_angerror_temp >= -3)
    {
        turn_p= 15;  //���ֵ��Ҫ������Сs���������ʹ����ֱ��ͨ��;
     }
    else
    {
        turn_p = a*(g_angerror_temp*g_angerror_temp)+ b; 
    }
    */
  
    //����3 ��ֱ�Ӽ��������жϱ�־λ��������ʱ�������ӵ���ʻ�켣������ƫ������һ��
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

    //��������
    SET_PWM_SVO(PWM_turn_value);

}
//ccd����
/*************************************************************************
*                           ������ӹ�����
*
*  �������ƣ�SendImageData
*  ����˵����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2012-10-20
*  ��    ע��
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
*                           ������ӹ�����
*
*  �������ƣ�SendHex
*  ����˵�����ɼ���������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2012-10-20
*  ��    ע��
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