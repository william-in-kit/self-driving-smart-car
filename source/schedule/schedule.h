#ifndef __SENSORCOLLECT_H__
#define __SENSORCOLLECT_H__

/******************************************************************************/
/*----------------------- M A C R O S ----------------------------------------*/



/*----------------------- V A R I A B L E S --------------------------*/
//extern signed int g_angerror_temp ;
extern int16 g_motor_set;
extern int16 g_motor_set_turn;
extern int16 g_servo_set;
extern int g_speedvalue_temp;
extern int g_ccd_sendflag;
extern int16 slow_down_rate;
extern int16 g_speed_feedback;
extern uint8 g_speed_counter;
extern uint8 g_sendflag;
extern uint8 g_lcdflag;
extern uint8 middle;
extern unsigned int PWM_turn_value;

/*----------------------- Function prototypes --------------------------*/
void pit0_isr(void);
void Car_Run(void);
void Car_Turn(void);
void SendImageData(char ph[]) ;
void SendHex(unsigned char hex) ;
/**********************************************************************/
#endif
