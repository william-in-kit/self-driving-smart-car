
#include "common.h"
#include "ccdidentify.h"

/***********************************************************/
#define g_noisevalue 255
#define shield_point 15     //���������

/***********************************************************/
//�ع�ʱ��
int exposure_time = 5;  //�ع�ʱ�䣬Ĭ��5ms�ع�һ��
//CCD����
uint8 g_ccd_middle = 63;  //63
int threshold = 50;
//ͼ��ʶ�����
uint8 left_toggle_point;
uint8 right_toggle_point;
signed int g_angerror_temp = 0;
signed int last_g_angerror_temp = 0;
uint8 middle_temp = 63;

int width=0;       //��·���
int width_last=0;

/******************************************************************************/
//CCDͼ��ʶ��
uint8 CCD_Identify(CCD_data *ccd)
{
    int k;

    //ȥ�������
    for(k = shield_point; k < GRAPH_WIDTH - shield_point - 1; k++)
    {
        if(ABS(ccd->graph[k] - ccd->graph[k-1]) > g_noisevalue
         &&ABS(ccd->graph[k] - ccd->graph[k+1]) > g_noisevalue)
        {
            ccd->graph[k] = (ccd->graph[k-1] + ccd->graph[k+1]) / 2;
        }
    }

    //�仯�ʷ�����������
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
   �뷨����right_toggle_point - left_toggle_point���㵱ǰ��·��Ȳ����浽 width_last ��
   ��ccdʶ���ͼ����һ�ߵ���������ʧʱ��˵���˴�Ҫ������Ķ����ϴ�ԭ�㷨�е���ߵ���������ʧ
    ʱ�������������Ϊͼ��������14-113�������Сֵ14���ұ���ʧʱ�������Ϊͼ��������14-113���ұ����ֵ113���ұߣ���
    ������ʵ�ʲ�������Ϊʵ�ʵı����Ѿ�������һ���޶�̽�ⲻ���������ͻ�ʹ���ƫת�ĽǶȱ�Ӧ��ƫת�ĽǶ�С�����ܵ��³��ӳ���ܵ���
    ��ʱӦʹ����һ�λ�������������ʱ��·�Ŀ��������ʵ��������Ӧ����λ�ã��������������ֵ����ֻ��һ�������ص�����±�ԭ�㷨
    ��ƫ���Ӷ�ʹ���ƫ�Ƹ��ࡣ
 */
      //�����ϴε�·��ȣ����ڵ�ccdͼ��ֻ��һ�������ػ�û��ʱ������middle_temp��
    
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
    
//  ���ϼ�Ϊȫ�� 

    middle_temp = (left_toggle_point + right_toggle_point) / 2;

    return middle_temp;
}
