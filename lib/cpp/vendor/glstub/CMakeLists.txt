project(glstub)

file(GLOB_RECURSE SOURCES
  "include/*.h"
  "src/*.c"
)

add_library(glstub STATIC ${SOURCES})

target_include_directories(glstub PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(glstub PRIVATE
  android
  GLESv2
  EGL
)
