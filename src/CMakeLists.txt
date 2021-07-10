cmake_minimum_required(VERSION 3.8.0)
project(rplayer)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/ffmpeg)

file(GLOB_RECURSE SRC
  "${PROJECT_SOURCE_DIR}/*.h"
  "${PROJECT_SOURCE_DIR}/*.cpp"
  "${PROJECT_SOURCE_DIR}/**/*.h"
  "${PROJECT_SOURCE_DIR}/**/*.cpp"
)
add_library(rplayer SHARED ${SRC})
target_link_libraries(rplayer PUBLIC  
  android
  log
  z
  ffmpeg
  EGL
)
target_include_directories(rplayer
  PUBLIC ${PROJECT_SOURCE_DIR}/ffi
  PRIVATE ${PROJECT_SOURCE_DIR}
)
