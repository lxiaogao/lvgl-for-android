/**
	bugs fix:lxiaogao@163.com
**/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "lvgl/lvgl.h"

#define EVDEV_NAME "/dev/input/event3"

/**********************
 *  STATIC PROTOTYPES
 **********************/
int map(int x, int in_min, int in_max, int out_min, int out_max);

/**********************
 *  STATIC VARIABLES
 **********************/
int evdev_fd;
int evdev_root_x;
int evdev_root_y;
lv_indev_state_t evdev_button;

int evdev_key_val;

struct touch_mv
{
    int active_w;
    int active_h;
    int active_x;
    int active_y;
    bool is_active;
};
struct touch_mv t_mv
{
    .active_w = 100,
    .active_h = 100,
    .active_x = 0,
    .active_y = 0,
    .is_active = false
};

extern void set_surface_position(int x, int y);
/**
 * Initialize the evdev interface
 */
void evdev_init(void)
{
    evdev_fd = open(EVDEV_NAME, O_RDWR | O_NOCTTY | O_NDELAY);

    if (evdev_fd == -1)
    {
        perror("unable open evdev interface:");
        return;
    }

    fcntl(evdev_fd, F_SETFL, O_ASYNC | O_NONBLOCK);

    evdev_root_x = 0;
    evdev_root_y = 0;
    evdev_key_val = 0;
    evdev_button = LV_INDEV_STATE_REL;
}
/**
 * reconfigure the device file for evdev
 * @param dev_name set the evdev device filename
 * @return true: the device file set complete
 *         false: the device file doesn't exist current system
 */
bool evdev_set_file(char *dev_name)
{
    if (evdev_fd != -1)
    {
        close(evdev_fd);
    }

    evdev_fd = open(dev_name, O_RDWR | O_NOCTTY | O_NDELAY);

    if (evdev_fd == -1)
    {
        perror("unable open evdev interface:");
        return false;
    }

    fcntl(evdev_fd, F_SETFL, O_ASYNC | O_NONBLOCK);

    evdev_root_x = 0;
    evdev_root_y = 0;
    evdev_key_val = 0;
    evdev_button = LV_INDEV_STATE_REL;

    return true;
}

/**
 * Get the current position and state of the evdev
 * @param data store the evdev data here
 * @return false: because the points are not buffered, so no more data to be read
 */
bool evdev_read_imp(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    struct input_event in;

    while (read(evdev_fd, &in, sizeof(struct input_event)) > 0)
    {
        if (in.type == EV_REL)
        {
            if (in.code == REL_X)
#if EVDEV_SWAP_AXES
                evdev_root_y += in.value;
#else
                evdev_root_x += in.value;
#endif
            else if (in.code == REL_Y)
#if EVDEV_SWAP_AXES
                evdev_root_x += in.value;
#else
                evdev_root_y += in.value;
#endif
        }
        else if (in.type == EV_ABS)
        {
            if (in.code == ABS_X)
#if EVDEV_SWAP_AXES
                evdev_root_y = in.value;
#else
                evdev_root_x = in.value;
#endif
            else if (in.code == ABS_Y)
#if EVDEV_SWAP_AXES
                evdev_root_x = in.value;
#else
                evdev_root_y = in.value;
#endif
            else if (in.code == ABS_MT_POSITION_X)
#if EVDEV_SWAP_AXES
                evdev_root_y = in.value;
#else
                evdev_root_x = in.value;
#endif
            else if (in.code == ABS_MT_POSITION_Y)
#if EVDEV_SWAP_AXES
                evdev_root_x = in.value;
#else
                evdev_root_y = in.value;
#endif
            else if (in.code == ABS_MT_TRACKING_ID)
            {
                if (in.value == -1)
                    evdev_button = LV_INDEV_STATE_REL;
                else if (in.value == 0)
                    evdev_button = LV_INDEV_STATE_PR;
            }
        }
        else if (in.type == EV_KEY)
        {
            if (in.code == BTN_MOUSE || in.code == BTN_TOUCH)
            {
                if (in.value == 0)
                {
                    evdev_button = LV_INDEV_STATE_REL;
                    t_mv.is_active = false;
                }
                else if (in.value == 1)
                {
                    evdev_button = LV_INDEV_STATE_PR;
                    if ((evdev_root_x > t_mv.active_x) && (evdev_root_x < t_mv.active_x + t_mv.active_w) && (evdev_root_y > t_mv.active_y) && (evdev_root_y < t_mv.active_y + t_mv.active_h))
                    {
                        t_mv.is_active = true;
                    }
                }
            }
            else if (drv->type == LV_INDEV_TYPE_KEYPAD)
            {
                data->state = (in.value) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
                switch (in.code)
                {
                case KEY_BACKSPACE:
                    data->key = LV_KEY_BACKSPACE;
                    break;
                case KEY_ENTER:
                    data->key = LV_KEY_ENTER;
                    break;
                case KEY_UP:
                    data->key = LV_KEY_UP;
                    break;
                case KEY_LEFT:
                    data->key = LV_KEY_PREV;
                    break;
                case KEY_RIGHT:
                    data->key = LV_KEY_NEXT;
                    break;
                case KEY_DOWN:
                    data->key = LV_KEY_DOWN;
                    break;
                default:
                    data->key = 0;
                    break;
                }
                evdev_key_val = data->key;
                evdev_button = data->state;
                return false;
            }
        }
    }

    if (drv->type == LV_INDEV_TYPE_KEYPAD)
    {
        /* No data retrieved */
        data->key = evdev_key_val;
        data->state = evdev_button;
        return false;
    }
    if (drv->type != LV_INDEV_TYPE_POINTER)
        return false;
    /*Store the collected data*/

    if (t_mv.is_active == true)
    {
        set_surface_position(evdev_root_x, evdev_root_y);
        t_mv.active_x = evdev_root_x;
        t_mv.active_y = evdev_root_y;
        return false;
    }

#if EVDEV_CALIBRATE
    data->point.x = map(evdev_root_x, EVDEV_HOR_MIN, EVDEV_HOR_MAX, 0, drv->disp->driver->hor_res);
    data->point.y = map(evdev_root_y, EVDEV_VER_MIN, EVDEV_VER_MAX, 0, drv->disp->driver->ver_res);
#else
    data->point.x = evdev_root_x - t_mv.active_x;
    data->point.y = evdev_root_y - t_mv.active_y;
#endif

    data->state = evdev_button;

    if (data->point.x < 0)
        data->point.x = 0;
    if (data->point.y < 0)
        data->point.y = 0;
    if (data->point.x >= drv->disp->driver->hor_res)
        data->point.x = drv->disp->driver->hor_res - 1;
    if (data->point.y >= drv->disp->driver->ver_res)
        data->point.y = drv->disp->driver->ver_res - 1;

    return false;
}

void evdev_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    evdev_read_imp(drv, data);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}