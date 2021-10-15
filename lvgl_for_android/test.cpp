#include "lv_demo.h"
#include "lvgl/examples/lv_examples.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "stdio.h"

extern void surface_init(void);
extern void surface_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);
extern void evdev_init(void);
extern void evdev_read(lv_indev_drv_t * drv, lv_indev_data_t * data);
lv_obj_t * led1 ;

#define DISP_BUF_SIZE (128 * 1024)


static void btn_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = (lv_obj_t *)lv_event_get_user_data(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
         const char * state = lv_obj_get_state(obj) & LV_STATE_CHECKED ? "Checked" : "Unchecked";
        LV_LOG_USER("Clicked %s",state);
        lv_obj_set_size(obj,300,200);
        lv_obj_align(obj, LV_ALIGN_TOP_LEFT, 0, 0);
        lv_obj_set_pos(obj,100,100);
        if(lv_obj_get_state(obj) & LV_STATE_CHECKED)
            lv_led_on(led1);
        else
            lv_led_off(led1);
    } 
}


void demo_run(void)
{

    static lv_style_t style_main;
    lv_style_init(&style_main);
   // lv_style_set_bg_color(&style_main, lv_palette_lighten(LV_PALETTE_GREY, 2));
    lv_style_set_bg_opa(&style_main,LV_OPA_TRANSP); // set bg alpha to hide bg color
    lv_obj_add_style(lv_scr_act(),&style_main, 0);


    lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
    lv_obj_add_flag(btn1, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_event_cb(btn1, btn_event_handler, LV_EVENT_ALL, btn1);
    lv_obj_align(btn1, LV_ALIGN_TOP_MID, 0, 0);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, lv_palette_lighten(LV_PALETTE_LIGHT_GREEN, 2));
    lv_style_set_border_color(&style_btn, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_text_color(&style_btn, lv_palette_main(LV_PALETTE_RED));



    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
    lv_obj_align_to(btn2, btn1,LV_ALIGN_TOP_LEFT, 200, 100);
    label = lv_label_create(btn2);
    lv_label_set_text(label, "Button2");
    lv_obj_center(label);
    lv_obj_add_style(btn2, &style_btn, 0);

    /*Create a LED and switch it OFF*/
    led1  = lv_led_create(lv_scr_act());
    lv_obj_align(led1, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_led_off(led1);

}

int main(void)
{
    /*LittlevGL init*/
    lv_init();
    /*Linux frame buffer device init*/
    surface_init();
    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = surface_flush;
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 800;
    lv_disp_drv_register(&disp_drv);

    evdev_init();
    lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_drv_register(&indev_drv_1);

    /*Create a Demo*/
#if 1
    lv_demo_music();
#else
    demo_run();
#endif
    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_task_handler();
        usleep(5*1000);
        //lv_tick_inc(5);
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
