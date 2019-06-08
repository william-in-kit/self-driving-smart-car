/**************************************************************
** Copyright(c) 2011~2012 Smartcar Lab of USTB
** All Rights Reserve
**-------------------------------------------------------------
** MCU         : k60
** Crystal     :
** Module Name : ccdcollect.c
** Author      : 张云飞
** Date        : 2013.01.25
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
#include "ccdcollect.h"

/******************************************************************************/

/*--------------- V A R I A B L E S ------------------*/
CCD_data ccd0;

/*--------------- F U N C T I O N S ----------------------------------*/

/**************************************************************
Function Name  : CCD_collect
Date           : 2013.01.25
Description    :
Input	       : None
Output         : None
Notes	       :
说明           :需要在 SI 持续高电平 20ns后产生第 1个 CLK CLK信号，并在每
                个 CLK 信号的下降沿时采集 信号的下降沿时采集 AO 引脚的输
                出的电压值。在采集了128个像素后，还必须生成第129个CLK信号结束本次采集
**************************************************************/
void CCD_Collect(CCD_data *ccd)
{
  asm("nop");
  
  CCD_SI_LOW(ccd->id);
  asm("nop");
  
  CCD_SI_HIGH(ccd->id);            /* SI 高电平 */
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  
  CCD_CLK_HIGH(ccd->id);           /* CLK 高电平 */
  asm("nop");
  
  CCD_SI_LOW(ccd->id);            /* SI  低电平 */
  asm("nop");
  
  ccd->graph[0]=Get_AD_Channel(ccd->id);
  CCD_CLK_LOW(ccd->id);           /* CLK = 0 */
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");

  for(int i=1; i<19; i++)
  {
    asm("nop");
    asm("nop");
    CCD_CLK_HIGH(ccd->id);
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    ccd->graph[i] =  Get_AD_Channel(ccd->id);
    CCD_CLK_LOW(ccd->id);
  }
  
  asm("nop");
  
  for(int i=19; i<GRAPH_WIDTH; i++)
  {
    asm("nop");
    asm("nop");
    CCD_CLK_HIGH(ccd->id);       /* CLK 高电平 */
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    ccd->graph[i] =  Get_AD_Channel(ccd->id);
    CCD_CLK_LOW(ccd->id);        /* CLK 低电平 */
  }

    asm("nop");
    asm("nop");
    CCD_CLK_HIGH(ccd->id);           /* CLK 高电平 */
    asm("nop");
    asm("nop");
    CCD_CLK_LOW(ccd->id);           /* CLK 低电平 */
    asm("nop");
}

void CCD_Init(void)
{
  //CCD值初始化
  ccd0.id = 0;
}


