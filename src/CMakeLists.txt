cmake_minimum_required(VERSION 3.4.1)
project(rplayer)

add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/ffmpeg)

file(GLOB SRC
  "${PROJECT_SOURCE_DIR}/*.h"
  "${PROJECT_SOURCE_DIR}/*.cpp"
  "${PROJECT_SOURCE_DIR}/ffi/*.h"
  "${PROJECT_SOURCE_DIR}/ffi/*.cpp"
  "${PROJECT_SOURCE_DIR}/plugin/*.h"
  "${PROJECT_SOURCE_DIR}/plugin/*.cpp"
  "${PROJECT_SOURCE_DIR}/texture/*.h"
  "${PROJECT_SOURCE_DIR}/texture/*.cpp"
)
add_library(rplayer SHARED ${SRC})
target_link_libraries(rplayer PUBLIC ffmpeg)
target_include_directories(rplayer
  PUBLIC ${PROJECT_SOURCE_DIR}/ffi
  PRIVATE ${PROJECT_SOURCE_DIR}
)
