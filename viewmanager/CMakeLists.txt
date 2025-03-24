# 设置 cmake 最低版本
cmake_minimum_required(VERSION 3.5)

set(PROJ_NAME viewmanager)
project(${PROJ_NAME})

# 设置头文件路径
set(INC_LIST ${PROJROOT}/viewmanager/inc
             ${PROJROOT}/shader/inc
             ${PROJROOT}/imgtrans/inc
             ${PROJROOT}/thirdsdk/opencv-4.3.0/include)
# 设置源文件路径
set(SRC_LIST ${PROJROOT}/viewmanager/src/ViewManager.cpp
             ${PROJROOT}/viewmanager/src/Triangle.cpp
             ${PROJROOT}/viewmanager/src/Texture.cpp)
# 设置库文件路径
set(LIB_LIST "${PROJROOT}/build/lib/Debug"
             "${PROJROOT}/thirdsdk/opencv-4.3.0/lib")

# cmake 添加链接库路径
link_directories(${LIB_LIST})

# 设置编译选项
set(LOCAL_CXXFLAGS -fPIC -g)

# 设置编译
set(LIBTYPE SHARED) # STATIC SHARED

include_directories(${INC_LIST})
add_compile_options(${LOCAL_CXXFLAGS})

add_library(${PROJ_NAME} ${LIBTYPE} ${SRC_LIST})
target_link_libraries(${PROJ_NAME} PUBLIC ${LIB_LIST} -lshader -lGLESv2 -lEGL -lX11 -lopencv_core -lopencv_calib3d -lopencv_features2d -lopencv_imgproc -lopencv_videoio -lopencv_highgui)