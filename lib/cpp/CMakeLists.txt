cmake_minimum_required(VERSION 3.8.0)
project(rplayer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_subdirectory(vendor/ffmpeg)

if(ANDROID)
  add_subdirectory(vendor/glstub)

  file(GLOB_RECURSE SRC "*" "**/*")
  add_library(rplayer SHARED ${SRC})
  target_link_libraries(rplayer PRIVATE ffmpeg glstub)

  target_include_directories(rplayer PUBLIC include)
  target_include_directories(rplayer PRIVATE src)
endif()
