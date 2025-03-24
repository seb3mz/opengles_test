# 设置 cmake 最低版本
cmake_minimum_required(VERSION 3.5)

set(PROJ_NAME shader)
project(${PROJ_NAME})

# 设置源文件路径
set(SRC_LIST ${PROJROOT}/shader/src/Shader.cpp)

# 设置编译选项
set(LOCAL_CXXFLAGS -fPIC -g)

# 设置编译
set(LIBTYPE SHARED) # STATIC SHARED

add_compile_options(${LOCAL_CXXFLAGS})

add_library(${PROJ_NAME} ${LIBTYPE} ${SRC_LIST})