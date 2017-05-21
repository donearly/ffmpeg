cmake_minimum_required(VERSION 3.4.1)

add_library(avutil SHARED IMPORTED )
set_target_properties(
                        avutil
                        PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libavutil-55.so )

add_library(avcodec SHARED IMPORTED )
set_target_properties(
                        avcodec
                        PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libavcodec-57.so )

add_library(avfilter SHARED IMPORTED )
set_target_properties(
                        avfilter
                        PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libavfilter-6.so )

add_library(swresample SHARED IMPORTED )
set_target_properties(
                        swresample
                        PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libswresample-2.so )

add_library(swscale SHARED IMPORTED )
set_target_properties(
                        swscale
                        PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libswscale-4.so )

add_library(avformat SHARED IMPORTED )
set_target_properties(
                        avformat
                        PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libavformat-57.so )

add_library(avdevice SHARED IMPORTED )
set_target_properties(
                        avdevice
                        PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libavdevice-57.so )


include_directories(src/main/cpp/include)

add_library(
            native-lib
            SHARED
            src/main/cpp/native-lib.cpp )
find_library(
            log-lib
            log )


target_link_libraries(
                        native-lib
                        avutil
                        avcodec
                        avfilter
                        swresample
                        swscale
                        avformat
                        avdevice
                        ${log-lib} )