/*
 * File:	gpio.h
 * Purpose:
 *
 * Notes:
 */

#ifndef __GPIO_H__
#define __GPIO_H__

/* ----------------------------F U N C T I O N--------------------------------*/
// Function prototypes
void gpio_init(void);

/* ------------------------------M A C R O S----------------------------------*/
// Definition

/* 拨码开关, 此处保留，管脚分别为PB10、PB11、PB17、PB16 */
#define SWITCH1_PIN    (10)
#define SWITCH1_PDIR   GPIOC_PDIR
#define SWITCH1_PDDR   GPIOC_PDDR
#define SWITCH1_PCR    PORTC_PCR10

#define SWITCH2_PIN    (8)
#define SWITCH2_PDIR   GPIOC_PDIR
#define SWITCH2_PDDR   GPIOC_PDDR
#define SWITCH2_PCR    PORTC_PCR8

#define SWITCH3_PIN    (0)
#define SWITCH3_PDIR   GPIOD_PDIR
#define SWITCH3_PDDR   GPIOD_PDDR
#define SWITCH3_PCR    PORTD_PCR0

#define SWITCH4_PIN    (6)
#define SWITCH4_PDIR   GPIOC_PDIR
#define SWITCH4_PDDR   GPIOC_PDDR
#define SWITCH4_PCR    PORTC_PCR6


/******************************************************************************/
/* 键盘 */
#define ZLG7290_INT_PIN    (3)
#define ZLG7290_INT_PDIR   GPIOD_PDIR
#define ZLG7290_INT_PDDR   GPIOD_PDDR
#define ZLG7290_INT_PCR    PORTD_PCR3

#define ZLG7290_INT()   BIT_IS_HIGH(ZLG7290_INT_PDIR,ZLG7290_INT_PIN)

/* CCD */

#define CCD_0_SI_PIN   (7)
#define CCD_0_SI_DOR   GPIOC_PDOR
#define CCD_0_SI_DDR   GPIOC_PDDR
#define CCD_0_SI_PCR   PORTC_PCR7

#define CCD_0_CLK_PIN   (9)
#define CCD_0_CLK_DOR   GPIOC_PDOR
#define CCD_0_CLK_DDR   GPIOC_PDDR
#define CCD_0_CLK_PCR   PORTC_PCR9

/* LED, output , PC8,PC6,PC7,PC9*/
#define LED0_PIN    (14)
#define LED0_PDOR   GPIOC_PDOR
#define LED0_PDDR   GPIOC_PDDR
#define LED0_PCR    PORTC_PCR14

#define LED1_PIN    (15)
#define LED1_PDOR   GPIOC_PDOR
#define LED1_PDDR   GPIOC_PDDR
#define LED1_PCR    PORTC_PCR15

#define LED2_PIN    (12)
#define LED2_PDOR   GPIOC_PDOR
#define LED2_PDDR   GPIOC_PDDR
#define LED2_PCR    PORTC_PCR12

#define LED3_PIN    (13)
#define LED3_PDOR   GPIOC_PDOR
#define LED3_PDDR   GPIOC_PDDR
#define LED3_PCR    PORTC_PCR13

/* ---------------------------I N T E R F A C E-------------------------------*/
/* Interface */
#define SWITCH1_STATUS           BIT_IS_HIGH(SWITCH1_PDIR, SWITCH1_PIN)
#define SWITCH2_STATUS           BIT_IS_HIGH(SWITCH2_PDIR, SWITCH2_PIN)
#define SWITCH3_STATUS           BIT_IS_HIGH(SWITCH3_PDIR, SWITCH3_PIN)
#define SWITCH4_STATUS           BIT_IS_HIGH(SWITCH4_PDIR, SWITCH4_PIN)

/* CCD的 SI 和 CLK 高低宏定义*/
#define CCD_SI_HIGH(n)        SET_BIT(CCD_0_SI_DOR,CCD_0_SI_PIN);

#define CCD_SI_LOW(n)         CLR_BIT(CCD_0_SI_DOR,CCD_0_SI_PIN);

#define CCD_CLK_HIGH(n)       SET_BIT(CCD_0_CLK_DOR,CCD_0_CLK_PIN);

#define CCD_CLK_LOW(n)        CLR_BIT(CCD_0_CLK_DOR,CCD_0_CLK_PIN);

#define LED0_OFF()             SET_BIT(LED0_PDOR, LED0_PIN)
#define LED0_ON()              CLR_BIT(LED0_PDOR, LED0_PIN)
#define LED0_REV()             REV_BIT(LED0_PDOR, LED0_PIN)

#define LED1_OFF()             SET_BIT(LED1_PDOR, LED1_PIN)
#define LED1_ON()              CLR_BIT(LED1_PDOR, LED1_PIN)
#define LED1_REV()             REV_BIT(LED1_PDOR, LED1_PIN)

#define LED2_OFF()             SET_BIT(LED2_PDOR, LED2_PIN)
#define LED2_ON()              CLR_BIT(LED2_PDOR, LED2_PIN)
#define LED2_REV()             REV_BIT(LED2_PDOR, LED2_PIN)

#define LED3_OFF()             SET_BIT(LED3_PDOR, LED3_PIN)
#define LED3_ON()              CLR_BIT(LED3_PDOR, LED3_PIN)
#define LED3_REV()             REV_BIT(LED3_PDOR, LED3_PIN)

/******************************************************************************/
#endif /* __GPIO_H__ */
