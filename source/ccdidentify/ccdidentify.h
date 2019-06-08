#ifndef __CCDIDENTIFY_H__
#define __CCDIDENTIFY_H__



/*****************************************************************/
//extern
extern uint8 g_ccd_middle;
extern signed int g_angerror_temp;
extern signed int last_g_angerror_temp;
extern int exposure_time;       //曝光时间，单位: ms
extern uint8 left_toggle_point;
extern uint8 right_toggle_point;

/*****************************************************************/
//function
uint8 CCD_Identify(CCD_data *ccd);

/*****************************************************************/

#endif
