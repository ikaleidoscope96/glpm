cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(glpm)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image-shared)
find_package(SDL3_mixer REQUIRED CONFIG REQUIRED COMPONENTS SDL3_mixer-shared)
find_package(SDL3_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL3_ttf-shared)

add_executable(glpm
  src/main.cpp
  src/App.cpp
  src/Texture.cpp
)

target_include_directories(glpm PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(glpm PRIVATE
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_mixer::SDL3_mixer
  SDL3_ttf::SDL3_ttf
)
