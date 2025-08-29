# 设置 cmake 最低版本
cmake_minimum_required(VERSION 3.5)

set(PROJ_NAME tinyxml)
project(${PROJ_NAME})

# 设置源文件路径
set(INC_LIST "${PROJROOT}/thirdsdk/TinyXML/include"
             )

set(SRC_LIST "${PROJROOT}/thirdsdk/TinyXML/src/tinystr.cpp"
             "${PROJROOT}/thirdsdk/TinyXML/src/tinyxml.cpp"
             "${PROJROOT}/thirdsdk/TinyXML/src/tinyxmlerror.cpp"
             "${PROJROOT}/thirdsdk/TinyXML/src/tinyxmlparser.cpp"
             )

# 设置编译选项
set(LOCAL_CXXFLAGS -fPIC -g)

# 设置编译
set(LIBTYPE SHARED) # STATIC SHARED

add_compile_options(${LOCAL_CXXFLAGS})

add_library(${PROJ_NAME} ${LIBTYPE} ${SRC_LIST})
target_link_libraries(${PROJ_NAME})