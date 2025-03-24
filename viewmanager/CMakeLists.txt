# 设置 cmake 最低版本
cmake_minimum_required(VERSION 3.5)

set(PROJ_NAME viewmanager)
project(${PROJ_NAME})

# 设置源文件路径
set(SRC_LIST ${PROJROOT}/viewmanager/src/ViewManager.cpp
             ${PROJROOT}/viewmanager/src/Triangle.cpp
             ${PROJROOT}/viewmanager/src/Texture.cpp)

# 设置编译选项
set(LOCAL_CXXFLAGS -fPIC -g)

# 设置动态库编译选项，使用gold链接器
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold")

# 设置编译
set(LIBTYPE SHARED) # STATIC SHARED

add_compile_options(${LOCAL_CXXFLAGS})

add_library(${PROJ_NAME} ${LIBTYPE} ${SRC_LIST})
target_link_libraries(${PROJ_NAME} -lshader -lopencv_core -lopencv_calib3d -lopencv_features2d -lopencv_imgproc -lopencv_videoio -lopencv_highgui  -lGLESv2)