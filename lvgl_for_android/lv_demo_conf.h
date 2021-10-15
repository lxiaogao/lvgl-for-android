/**
 * @file lv_example_conf.h
 *
 */

#ifndef LV_EX_CONF_H
#define LV_EX_CONF_H


/*******************
 * GENERAL SETTING
 *******************/
#define LV_EX_PRINTF       1       /*Enable printf-ing data in demoes and examples*/
#define LV_EX_KEYBOARD     1       /*Add PC keyboard support to some examples (`lv_drivers` repository is required)*/
#define LV_EX_MOUSEWHEEL   1       /*Add 'encoder' (mouse wheel) support to some examples (`lv_drivers` repository is required)*/

/*********************
 * DEMO USAGE
 *********************/

/*Show some widget*/
#define LV_USE_DEMO_WIDGETS        1
#if LV_USE_DEMO_WIDGETS
#define LV_DEMO_WIDGETS_SLIDESHOW  0
#endif

/*Printer demo, optimized for 800x480*/
#define LV_USE_DEMO_PRINTER   0

/*Demonstrate the usage of encoder and keyboard*/
#define LV_USE_DEMO_KEYPAD_AND_ENCODER    1

/*Benchmark your system*/
#define LV_USE_DEMO_BENCHMARK   1

/*Stress test for LVGL*/
#define LV_USE_DEMO_STRESS     1

#define LV_USE_DEMO_MUSIC      1
#if LV_USE_DEMO_MUSIC
# define LV_DEMO_MUSIC_LANDSCAPE    0
# define LV_DEMO_MUSIC_LARGE        0
#define LV_DEMO_MUSIC_AUTO_PLAY     0
#endif

#endif /*LV_EX_CONF_H*/

