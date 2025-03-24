# 设置 cmake 最低版本
cmake_minimum_required(VERSION 3.5)

set(PROJ_NAME xwindow)
project(${PROJ_NAME})

# 设置源文件路径
set(SRC_LIST ${PROJROOT}/xwindow/src/Xwindow.cpp)

# 设置编译选项
set(LOCAL_CXXFLAGS -fPIC -g)

# 设置编译
set(LIBTYPE SHARED) # STATIC SHARED

add_library(${PROJ_NAME} ${LIBTYPE} ${SRC_LIST})
target_link_libraries(${PROJ_NAME} -lX11)