cmake_minimum_required(VERSION 3.4.1)

project(ffmpeg)

set(FFMPEG_PATH ${PROJECT_SOURCE_DIR}/lib/${ANDROID_ABI})

# ffmpeg 
add_library(ffmpeg INTERFACE)
target_include_directories(ffmpeg INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(ffmpeg INTERFACE
  android
  log

  "${FFMPEG_PATH}/libavcodec.so"
  "${FFMPEG_PATH}/libavformat.so"
  "${FFMPEG_PATH}/libavutil.so"
  "${FFMPEG_PATH}/libswresample.so"
  "${FFMPEG_PATH}/libswscale.so"
)
