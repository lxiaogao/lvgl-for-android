1.compile liblvgl.a
	cd /external/lvgl/lvgl/build
	source /opt/cmake-3.21.1/env.sh 
	cmake -DCMAKE_TOOLCHAIN_FILE=../cross_compile.cmake ../
	make -j16

2.compile lvgl demo

mmm extern/lvgl/ -j16
