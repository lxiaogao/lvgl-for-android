# lvgl-for-android

#### 介绍
You can use C to develop HMI in android without java android-api , and can run app in cmdline directly. ideally situated for bsp and mcu engineers to develop HMI in android,who familiarize with lvgl

Now support the following feature:
1.support input dev such as touch-screen , can drag lvgl-app-window position in android-display
2.support alpha-channel,means lvgl app can display over or under other android-app and blend to display
3.remove hardware-accelerate code to make this project simplest and more generic to port

#### 软件架构
Based on lvgl8.0.0 ,you can upgrade by yourself


#### 安装教程

1.  download code to your android project external dir
![输入图片说明](https://images.gitee.com/uploads/images/2021/1015/135236_9d702f3f_143175.png "屏幕截图.png")

2.  compile liblvgl.a(or get liblvgl.a from me ,you can skip this step)

```
	cd /external/lvgl/lvgl/build
	source /opt/cmake-3.21.1/env.sh 
	cmake -DCMAKE_TOOLCHAIN_FILE=../cross_compile.cmake ../
	make -j16

```
note you need modify cross_compile.cmake according to your ndk-toolschain
![输入图片说明](https://images.gitee.com/uploads/images/2021/1015/135447_db4f7716_143175.png "屏幕截图.png")


3.  compile demo-app ,we support

#### 使用说明

1.  xxxx
2.  xxxx
3.  xxxx

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
