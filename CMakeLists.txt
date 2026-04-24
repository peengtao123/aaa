cmake_minimum_required(VERSION 3.10.0)
project(aaa VERSION 0.1.0 LANGUAGES C CXX)

# 查找 OpenCV 包
find_package(OpenCV REQUIRED)

# 包含 OpenCV 头文件
include_directories(${OpenCV_INCLUDE_DIRS})

# 使用 pkg-config 查找 GTK3 和 WebKit2GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-4.0)

# 包含 GTK 和 WebKit 头文件
include_directories(${GTK3_INCLUDE_DIRS} ${WEBKIT2_INCLUDE_DIRS})

add_executable(aaa main.cpp)

# 将 OpenCV, GTK 和 WebKit 库链接到可执行文件 aaa
target_link_libraries(aaa 
    ${OpenCV_LIBS} 
    ${GTK3_LIBRARIES}
    ${WEBKIT2_LIBRARIES}
)

# 添加 GTK 和 WebKit 所需的编译器标志
target_compile_options(aaa PRIVATE ${GTK3_CFLAGS_OTHER} ${WEBKIT2_CFLAGS_OTHER})