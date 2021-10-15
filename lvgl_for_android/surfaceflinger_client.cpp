/**
	bugs fix:lxiaogao@163.com
**/

#include "lvgl/lvgl.h"
#include <cutils/memory.h>
#include <utils/Log.h>
#include <binder/IServiceManager.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <android/native_window.h>
#include "stdio.h"

using namespace android;
#define SURFACE_W 1080
#define SURFACE_H 1920

/*
stride do not equal to width
768                    720
*/

static unsigned char *fbp = 0;
static uint32_t xoffset=0;
static uint32_t yoffset=0;
static uint32_t xres=SURFACE_W;
static uint32_t yres=SURFACE_H;
static uint32_t bits_per_pixel=32;
static uint32_t line_length=0;
/**********************
*   GLOBAL FUNCTIONS
**********************/

sp<Surface> surface ;
ANativeWindow_Buffer outBuffer;
SurfaceComposerClient::Transaction t;
sp<SurfaceControl> surfaceControl;

void surface_init(void)
{

    // 创建与Surfaceflinger通信的客户端
    sp<SurfaceComposerClient> client = new SurfaceComposerClient();
    // 创建SurfaceControl并设置名称“resize”、宽高、像素格式。
    surfaceControl = client->createSurface(String8("xili-Surface_Test"), SURFACE_W, SURFACE_H, PIXEL_FORMAT_RGBA_8888, 0);

    //通过SurfaceControl获取一个Surface
    surface = surfaceControl->getSurface();
    surface->setSharedBufferMode(true); // only using one buffer

    //设置Layer层级，数值越大层级越高
    t.setLayer(surfaceControl, 100000).apply();
    t.setPosition(surfaceControl, 0, 0).apply();
    client->incStrong((void*)surface_init);  //增加智能指针的引用计数，要不然指向的对象会被自动释放
    surfaceControl->incStrong((void*)surface_init);//增加智能指针的引用计数，要不然指向的对象会被自动释放
    surface->incStrong((void*)surface_init);//增加智能指针的引用计数，要不然指向的对象会被自动释放

}


void set_surface_position(int x , int y)
{
    t.setPosition(surfaceControl, x, y).apply();
}

void memcpy_argb_2_abgr(uint32_t *dst,uint32_t *src,int size)
{
    int i=0;
    for(i=0;i<size/4;i++)
        *(dst+i)=(*(src+i)&0xff00ff00) | ((*(src+i)&0xff)<<16) | ((*(src+i)&0xff0000)>>16) ;

}

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixel to copy to the `area` part of the screen
 */
void surface_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p)
{
    surface->lock(&outBuffer, NULL);
    //计算每个像素点的字节大小
    ssize_t bpr = outBuffer.stride * bytesPerPixel(outBuffer.format);

    fbp = (unsigned char *)outBuffer.bits;
    bits_per_pixel = bytesPerPixel(outBuffer.format) * 8;
    line_length = outBuffer.stride * bytesPerPixel(outBuffer.format);
    //printf("line_length:%d, fbp:0x%x, bits_per_pixel:%d\n",line_length,(unsigned char *)fbp,bits_per_pixel);

    if(fbp == NULL ||
            area->x2 < 0 ||
            area->y2 < 0 ||
            area->x1 > (int32_t)xres - 1 ||
            area->y1 > (int32_t)yres - 1) {
        lv_disp_flush_ready(drv);
        return;
    }

    /*Truncate the area to the screen*/
    int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t act_x2 = area->x2 > (int32_t)xres - 1 ? (int32_t)xres - 1 : area->x2;
    int32_t act_y2 = area->y2 > (int32_t)yres - 1 ? (int32_t)yres - 1 : area->y2;


    lv_coord_t w = (act_x2 - act_x1 + 1);
    long int location = 0;
    long int byte_location = 0;
    unsigned char bit_location = 0;

    /*32 or 24 bit per pixel*/
    if(bits_per_pixel == 32 || bits_per_pixel == 24) {
        uint32_t * fbp32 = (uint32_t *)fbp;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
            location = (act_x1 + xoffset) + (y + yoffset) * line_length / 4;
            memcpy_argb_2_abgr(&fbp32[location], (uint32_t *)color_p, (act_x2 - act_x1 + 1) * 4);
            color_p += w;
        }
    }
    /*16 bit per pixel*/
    else if(bits_per_pixel == 16) {
        uint16_t * fbp16 = (uint16_t *)fbp;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
            location = (act_x1 + xoffset) + (y + yoffset) * line_length / 2;
            memcpy(&fbp16[location], (uint32_t *)color_p, (act_x2 - act_x1 + 1) * 2);
            color_p += w;
        }
    }
    /*8 bit per pixel*/
    else if(bits_per_pixel == 8) {
        uint8_t * fbp8 = (uint8_t *)fbp;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
            location = (act_x1 + xoffset) + (y + yoffset) * line_length;
            memcpy(&fbp8[location], (uint32_t *)color_p, (act_x2 - act_x1 + 1));
            color_p += w;
        }
    }
    /*1 bit per pixel*/
    else if(bits_per_pixel == 1) {
        uint8_t * fbp8 = (uint8_t *)fbp;
        int32_t x;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
            for(x = act_x1; x <= act_x2; x++) {
                location = (x + xoffset) + (y + yoffset) * xres;
                byte_location = location / 8; /* find the byte we need to change */
                bit_location = location % 8; /* inside the byte found, find the bit we need to change */
                fbp8[byte_location] &= ~(((uint8_t)(1)) << bit_location);
                fbp8[byte_location] |= ((uint8_t)(color_p->full)) << bit_location;
                color_p++;
            }

            color_p += area->x2 - act_x2;
        }
    } else {
        /*Not supported bit per pixel*/
    }

    lv_disp_flush_ready(drv);

    surface->unlockAndPost();
}


