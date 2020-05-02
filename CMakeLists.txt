cmake_minimum_required(VERSION 3.15)
project(gstdemo C)

set(CMAKE_C_STANDARD 99)

# Required for GStreamer
find_package(PkgConfig)

# Look for GStreamer installation
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(GST REQUIRED gstreamer-plugins-base-1.0)
pkg_check_modules(GST REQUIRED gstreamer-pbutils-1.0)
pkg_check_modules(GST REQUIRED gstreamer-tag-1.0)

add_executable(gstdemo
        main.c
        Player.c
        MediaInfo.c
        PlayerMainContextSignalDispatcher.c
        PlayerSignalDispatcher.c)

# GStreamer
target_include_directories(gstdemo PUBLIC ${GST_INCLUDE_DIRS})
target_compile_options(gstdemo PUBLIC ${GST_CFLAGS})
target_link_libraries(gstdemo ${GST_LINK_LIBRARIES})

