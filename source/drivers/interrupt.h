/*
 * File:		interrupt.h
 * Purpose:
 *
 * Notes:
 */

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
/******************************************************************************/
/* ARM Cortex M4 implementation for interrupt priority shift */
#define ARM_INTERRUPT_LEVEL_BITS    4

/******************************************************************************/
// Define
#define INTERRUPT_PIT0_IRQ      (68) // PIT0
#define INTERRUPT_PIT1_IRQ      (69) // PIT1

/******************************************************************************/
/* Set Interrupt Vector Table Function Pointers */
#undef  VECTOR_084    //INTERRUPT_PIT0_IRQ + 16   //ȡ��֮ǰ�Ķ���
#define VECTOR_084  pit0_isr    //���¶����ж��������е�PIT Channel 0����Ӧ���жϷ������ĵ�ַ
//#undef  VECTOR_085    //INTERRUPT_PIT1_IRQ + 16   //ȡ��֮ǰ�Ķ���
//#define VECTOR_085  pit1_isr    //���¶����ж��������е�PIT Channel 1����Ӧ���жϷ������ĵ�ַ

/******************************************************
���ߣ���ΰ��
���ڣ�2014��1��22��
ʹ��˵������Ҫ�õ���GPIO�жϣ�������жϺ������󣬽��ò����жϡ�
          ճ����interrupt.h��ȡ��ע�ͣ��ж�������Ӧ�ĺ������vector.h
//GPIOģ���жϷ�����
#undef  VECTOR_103       //ȡ��֮ǰ�Ķ���
#define VECTOR_103  pit0_isr    //���¶����ж��������е�PORTA����Ӧ���жϷ������ĵ�ַ

#undef  VECTOR_104       //ȡ��֮ǰ�Ķ���
#define VECTOR_104  pit0_isr    //���¶����ж��������е�PORTB����Ӧ���жϷ������ĵ�ַ

#undef  VECTOR_105       //ȡ��֮ǰ�Ķ���
#define VECTOR_105  pit0_isr    //���¶����ж��������е�PORTC����Ӧ���жϷ������ĵ�ַ

#undef  VECTOR_106       //ȡ��֮ǰ�Ķ���
#define VECTOR_106  pit0_isr    //���¶����ж��������е�PORTD����Ӧ���жϷ������ĵ�ַ

#undef  VECTOR_107       //ȡ��֮ǰ�Ķ���
#define VECTOR_107  pit0_isr    //���¶����ж��������е�PORTE����Ӧ���жϷ������ĵ�ַ

......
......
******************************************************/
// Function prototypes
void Enable_irq(int);
void Disable_irq(int);
void Set_irq_Priority(int, int);
void Interrupt_Init(void);

/******************************************************************************/
#endif /* __INTERRUPT_H__ */