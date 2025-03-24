# 设置 cmake 最低版本
cmake_minimum_required(VERSION 3.5)

set(PROJ_NAME imgtrans)
project(${PROJ_NAME})

# 设置源文件路径
set(SRC_LIST ${PROJROOT}/imgtrans/src/imgtrans.cpp)

# 设置编译选项
set(LOCAL_CXXFLAGS -fPIC -g)

# 设置编译
set(LIBTYPE SHARED) # STATIC SHARED

include_directories(${INC_LIST})
add_compile_options(${LOCAL_CXXFLAGS})

add_library(${PROJ_NAME} ${LIBTYPE} ${SRC_LIST})