
#include "common.h"
#include "ccdidentify.h"

/***********************************************************/
#define g_noisevalue 255
#define shield_point 15     //两侧的屏蔽

/***********************************************************/
//曝光时间
int exposure_time = 5;  //曝光时间，默认5ms曝光一次
//CCD变量
uint8 g_ccd_middle = 63;  //63
int threshold = 50;
//图像识别参数
uint8 left_toggle_point;
uint8 right_toggle_point;
signed int g_angerror_temp = 0;
signed int last_g_angerror_temp = 0;
uint8 middle_temp = 63;

int width=0;       //道路宽度
int width_last=0;

/******************************************************************************/
//CCD图像识别
uint8 CCD_Identify(CCD_data *ccd)
{
    int k;

    //去单个噪点
    for(k = shield_point; k < GRAPH_WIDTH - shield_point - 1; k++)
    {
        if(ABS(ccd->graph[k] - ccd->graph[k-1]) > g_noisevalue
         &&ABS(ccd->graph[k] - ccd->graph[k+1]) > g_noisevalue)
        {
            ccd->graph[k] = (ccd->graph[k-1] + ccd->graph[k+1]) / 2;
        }
    }

    //变化率方法找跳变沿
    for(k = 63; k > shield_point; k--)
    {
        if((ccd->graph[k] - ccd->graph[k-3]) >= threshold)
        {
            break;
        }
    }

    left_toggle_point = k - 1;

    for(k = 64; k < GRAPH_WIDTH - 1 - shield_point; k++)
    {
        if((ccd->graph[k] - ccd->graph[k+3]) >= threshold)
        {
            break;
        }
    }

    right_toggle_point = k + 1;
    
    
    /*   
   想法：用right_toggle_point - left_toggle_point计算当前道路宽度并保存到 width_last 。
   当ccd识别的图像有一边的跳变沿消失时，说明此处要过的弯的度数较大，原算法中当左边的跳变沿消失
    时计算出的跳变沿为图像处理区（14-113）左侧最小值14，右边消失时计算出的为图像处理区（14-113）右边最大值113（右边），
    这样与实际不符，因为实际的边沿已经超过这一界限而探测不到。这样就会使舵机偏转的角度比应该偏转的角度小，可能导致车子冲出跑道。
    此时应使用上一次还有两个跳变沿时道路的宽度来计算实际跳变沿应处的位置，这样计算出的中值会在只有一个跳变沿的情况下比原算法
    更偏，从而使舵机偏移更多。
 */
      //保存上次道路宽度，用于当ccd图像只有一个跳变沿或没有时，计算middle_temp用
    
    width = right_toggle_point - left_toggle_point;
    width_last=width;
    //if((left_toggle_point>shield_point + 1)&&(right_toggle_point < GRAPH_WIDTH - 1 - shield_point - 1 ))
    //{
    //   width_last = width;
    //}
    
     if(left_toggle_point <= shield_point + 2)
    {
       left_toggle_point =  right_toggle_point - width_last;
    }
    
    
    if(right_toggle_point >= GRAPH_WIDTH - 1 - shield_point - 1 )
    {
        right_toggle_point = left_toggle_point + width_last;
    }
    
//  以上即为全部 

    middle_temp = (left_toggle_point + right_toggle_point) / 2;

    return middle_temp;
}
