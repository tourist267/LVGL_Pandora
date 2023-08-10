#include "user_lvgl.h"
#include "lv_obj.h"
#include "lv_port_disp.h"
#include "lv_example_get_started.h"
#include "../lv_examples.h"


void LVGL_CentralButton(void)
{
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_height(btn, 30);
 
    lv_obj_t *label;
    label = lv_label_create(btn);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label, "LVGL");
 
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);
    lv_style_set_border_color(&style_btn, lv_color_white());
    lv_style_set_border_opa(&style_btn, LV_OPA_30);
    lv_obj_add_style(btn, &style_btn, LV_STATE_DEFAULT);
}
 
void LVGL_Init(void)
{
    lv_init();                  // lvgl��ʼ����������û�г�ʼ������ô����ĳ�ʼ�������
    lv_port_disp_init();        // ��ʾ����ʼ��
    // lv_port_indev_init();       // �����豸��ʼ�������û��ʵ�־�ע�͵���
    // lv_port_fs_init();          // �ļ�ϵͳ�豸��ʼ�������û��ʵ�־�ע�͵���
}


void TestExample(void)
{
	//lv_example_get_started_1();
	lv_example_anim_1();
}


