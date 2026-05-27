cmake_minimum_required(VERSION 3.10)
project(myblog)

# 编译模式
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -Wall -Werror")

# 依赖库查找
find_package(PkgConfig REQUIRED)

find_path(TIDE_INCLUDE_DIR NAMES tide/tide.h REQUIRED)
find_library(TIDE_LIBRARY NAMES tide REQUIRED)

# vcpkg sqlite3
set(VCPKG_ROOT $ENV{HOME}/install/vcpkg)
set(VCPKG_INSTALLED_PATH ${VCPKG_ROOT}/installed/x64-linux)

# 构建目标
set(LIB_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/blog/my_module.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/orm_out/user_info.cc
)
add_library(myblog SHARED ${LIB_SRC})

target_include_directories(myblog PRIVATE
    ${TIDE_INCLUDE_DIR}
    ${VCPKG_INSTALLED_PATH}/include
)
link_directories(${VCPKG_INSTALLED_PATH}/lib)
target_link_libraries(myblog
    PUBLIC
    ${TIDE_LIBRARY}
    sqlite3
)

# 输出目录
set_target_properties(myblog PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)