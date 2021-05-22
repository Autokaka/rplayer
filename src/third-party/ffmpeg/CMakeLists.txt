cmake_minimum_required(VERSION 3.4.1)
project(ffmpeg)

set(FFMPEG_PATH ${PROJECT_SOURCE_DIR}/${ANDROID_ABI})

add_library(ffmpeg INTERFACE)
target_include_directories(ffmpeg INTERFACE ${FFMPEG_PATH}/include)
target_link_libraries(ffmpeg INTERFACE
  "${FFMPEG_PATH}/libavformat.a"
  "${FFMPEG_PATH}/libavcodec.a"
  "${FFMPEG_PATH}/libavutil.a"
  "${FFMPEG_PATH}/libswresample.a"
  "${FFMPEG_PATH}/libswscale.a"
)
