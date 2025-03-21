# 设置 cmake 最低版本
cmake_minimum_required(VERSION 3.5)

# 设置项目名称
set(PROJ_NAME xstart)
project (${PROJ_NAME})

message("${PROJ_NAME} test begin Make")

# 设置顶层路径
set(PROJROOT ${CMAKE_SOURCE_DIR})

# 设置编译生成文件类型
set(CMAKE_BUILD_TYPE Debug)
#静态库生成路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJROOT}/build/lib/Debug)   
# 动态库生成路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJROOT}/build/lib/Debug)
#可执行文件生成路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJROOT}/build/bin/Debug)   

# 设置头文件路径
set(INC_LIST "${PROJROOT}/eglsurface/inc"
             "${PROJROOT}/viewmanager/inc"
             "${PROJROOT}/shader/inc"
             "${PROJROOT}/imgtrans/inc"
             "${PROJROOT}/thirdsdk/opencv-4.3.0/include"
             "${PROJROOT}/xwindow/inc")
# 设置源文件路径
set(SRC_LIST "./main.cpp")
# 设置库文件路径
set(LIB_LIST "${PROJROOT}/build/lib/Debug"
             "${PROJROOT}/thirdsdk/opencv-4.3.0/lib")

# cmake 添加头文件路径
include_directories(${INC_LIST})

# cmake 添加链接库路径
link_directories(${LIB_LIST})

# cmake_policy(SET CMP0072 NEW)  # 启用GLVND优先
# set(OpenGL_GL_PREFERENCE "GLVND")  # 显式要求使用GLVND库
# find_package(OpenGL REQUIRED)


add_subdirectory(${PROJROOT}/eglsurface)
add_subdirectory(${PROJROOT}/shader)
add_subdirectory(${PROJROOT}/viewmanager)
add_subdirectory(${PROJROOT}/imgtrans)
add_subdirectory(${PROJROOT}/xwindow)

# 设置编译选项
set(LOCAL_CXXFLAGS -std=c++11)
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=gold")

# cmake 添加编译选项
add_compile_options(${LOCAL_CXXFLAGS})

# cmake 添加可执行文件
add_executable(${PROJ_NAME} ${SRC_LIST})
# 添加编译依赖
add_dependencies(${PROJ_NAME} eglsurface)
add_dependencies(${PROJ_NAME} shader)
add_dependencies(${PROJ_NAME} viewmanager)
add_dependencies(${PROJ_NAME} imgtrans)
add_dependencies(${PROJ_NAME} xwindow)

# cmake 向可执行文件添加链接库
target_link_libraries(${PROJ_NAME} libeglsurface.so libviewmanager.so libshader.so libimgtrans.so libxwindow.so -lopencv_core -lopencv_calib3d -lopencv_features2d -lopencv_imgproc -lopencv_videoio -lopencv_highgui libEGL.so libGLESv2.so -lX11)