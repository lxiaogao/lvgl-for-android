LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS  := lvgl/build/liblvgl.a 

include $(BUILD_MULTI_PREBUILT)


include $(CLEAR_VARS)





FILE_LIST := $(wildcard $(LOCAL_PATH)/lv_demo_music/assets/*.c)  
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)


LOCAL_SRC_FILES += \
    lv_demo_music/lv_demo_music.cpp \
    lv_demo_music/lv_demo_music_list.cpp \
    lv_demo_music/lv_demo_music_main.cpp \
    surfaceflinger_client.cpp \
    evdev.cpp \
    test.cpp 

LOCAL_C_INCLUDES += \
                  frameworks/native/libs/nativewindow/include/ \

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libbinder \
    libui \
    libgui \
        libEGL \
        libGLESv2 \
        libdng_sdk \
        libexpat \
        libft2 \
        libheif \
        libicui18n \
        libicuuc \
        libjpeg \
        liblog \
        libpiex \
        libpng \
        libvulkan \
        libz \
        libnativewindow \

LOCAL_STATIC_LIBRARIES += \
        libarect \
        libsfntly \
        libwebp-decode \
        libwebp-encode \
        liblvgl 


LOCAL_MODULE:= lvgl

LOCAL_MODULE_TAGS := tests
LOCAL_CFLAGS += -Wno-unused-result
LOCAL_CFLAGS += -Wno-unused-variable
LOCAL_CFLAGS += -Wno-sign-compare
LOCAL_CFLAGS += -Wno-pointer-sign
LOCAL_CFLAGS += -Wno-unused-function
LOCAL_CFLAGS += -Wno-unused-parameter
LOCAL_CFLAGS += -Wno-unused-variable
LOCAL_CFLAGS += -Wno-implicit-function-declaration
LOCAL_CFLAGS += -Wno-unused-result
LOCAL_CFLAGS += -Wno-pointer-arith
LOCAL_CFLAGS += -Wno-pointer-bool-conversion
LOCAL_CFLAGS += -Wno-incompatible-pointer-types-discards-qualifiers
LOCAL_CFLAGS += -Wno-format
include $(BUILD_EXECUTABLE)
