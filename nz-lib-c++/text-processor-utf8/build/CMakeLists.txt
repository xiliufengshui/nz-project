# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (text-processor)

message("-----------------------------------------------------")
message(STATUS "project source: " ${PROJECT_SOURCE_DIR})
message(STATUS "project binary: " ${PROJECT_BINARY_DIR})
# message(STATUS "opencv include path: " ${OpenCV_INCLUDE_DIRS})
# message(STATUS "opencv libs: " ${OpenCV_LIBS})

# 定义变量
# set (LIB_ZJ opencv_calib3d )

# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set (NZ_BIN_OUTPUT test.out)
set (NZ_DIR_SRC ../src)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)

# 编译选项
add_compile_options(-fPIC)

# 查找目录下的所有源文件
foreach (SUB_DIR ${NZ_DIR_SRC})
aux_source_directory (${SUB_DIR} TMP_SUB_SRC)
set (NZ_SRC ${TMP_SUB_SRC})
endforeach (SUB_DIR)
# aux_source_directory(. DIR_SRCS)
message(STATUS "nz_src: " ${NZ_SRC})
message("----------------------------------------------------")


# 添加 math 子目录 生成库
# add_subdirectory(math)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
# aux_source_directory(. DIR_LIB_SRCS)
# 生成链接库
# add_library (MathFunctions ${DIR_LIB_SRCS})

# 生成链接
set (LIB_SRC ${PROJECT_SOURCE_DIR}/../src/text_processor.cpp)
ADD_LIBRARY (text_processor STATIC ${LIB_SRC})

# 链接
# include_directories (${DIR_HEAD})
# link_directories (${DIR_LIB})

# 指定生成目标
add_executable(${NZ_BIN_OUTPUT} ${NZ_SRC})

# 添加链接库
# target_link_libraries (${BIN_OUTPUT_ZJ} ${LIB_ZJ})
