cmake_minimum_required(VERSION 3.4.1)

project(rplayer)

add_subdirectory(${PROJECT_SOURCE_DIR}/ffmpeg)

aux_source_directory(${PROJECT_SOURCE_DIR} SRC)
add_library(rplayer SHARED ${SRC})
target_link_libraries(rplayer PUBLIC ffmpeg)
