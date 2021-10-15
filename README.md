# lvgl-for-android

#### 介绍
You can use C to develop HMI in android without java android-api , and can run app in cmdline directly. ideally situated for bsp and mcu engineers to develop HMI in android,who familiarize with lvgl

Now support the following feature:

- support input dev such as touch-screen , can drag lvgl-app-window position in android-disp1. 
- support alpha-channel,means lvgl app can display over or under other android-app and blend to display
- remove hardware-accelerate code to make this project simplest and more generic to port



#### 软件架构
Based on lvgl8.0.0 ,you can upgrade by yourself


#### 安装教程

1.  Download code to your android project external dir
![输入图片说明](https://images.gitee.com/uploads/images/2021/1015/135236_9d702f3f_143175.png "屏幕截图.png")

2.  Compile liblvgl.a(or get liblvgl.a from me ,you can skip this step),need cmake,recommend cmake-3.21.1

```
	cd /external/lvgl/lvgl/build
	source /opt/cmake-3.21.1/env.sh 
	cmake -DCMAKE_TOOLCHAIN_FILE=../cross_compile.cmake ../
	make -j16

```
Note you need modify cross_compile.cmake according to your ndk-toolschain
![输入图片说明](https://images.gitee.com/uploads/images/2021/1015/135447_db4f7716_143175.png "屏幕截图.png")


3.  Compile demo-app by android-mk/mmm ,we support two demo to show you,how to use lvgl-api in android 

```
mmm ./external/lvgl/ -j16

```
![输入图片说明](https://images.gitee.com/uploads/images/2021/1015/141447_b23c12f7_143175.png "屏幕截图.png")

#### 移植使用说明

1.  Change input dev node according to your platform,just modify evdev.cpp
![输入图片说明](https://images.gitee.com/uploads/images/2021/1015/141723_6b57a87f_143175.png "屏幕截图.png")
2.  Change demo1 and demo2 to compile,just modify test.cpp
![输入图片说明](https://images.gitee.com/uploads/images/2021/1015/141949_df5dbad0_143175.png "屏幕截图.png")

#### 参与贡献

Welcome to use this project in your product ,such as factory-mode-hmi,selftest-hmi by bsp-driver , fastcamera-hmi


#### 图片演示