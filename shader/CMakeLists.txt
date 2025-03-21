# 设置 cmake 最低版本
cmake_minimum_required(VERSION 3.5)

set(PROJ_NAME shader)
project(${PROJ_NAME})

# 设置头文件路径
set(INC_LIST ${PROJROOT}/shader/inc)
# 设置源文件路径
set(SRC_LIST ${PROJROOT}/shader/src/Shader.cpp)
# 设置库文件路径
set(LIB_LIST "./lib")

# 设置编译选项
set(LOCAL_CXXFLAGS -fPIC -g)

# 设置编译
set(LIBTYPE SHARED) # STATIC SHARED

include_directories(${INC_LIST})
add_compile_options(${LOCAL_CXXFLAGS})

add_library(${PROJ_NAME} ${LIBTYPE} ${SRC_LIST})