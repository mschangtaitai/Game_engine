cmake_minimum_required(VERSION 3.23)

project(IsEpic VERSION 1.0)

add_executable(${PROJECT_NAME} src/main.cpp)

include_directories(src)
add_subdirectory(src/Game)
add_subdirectory(src/Scene)

target_include_directories(${PROJECT_NAME}
  PUBLIC src/Game
  PUBLIC src/Scene
)

target_link_libraries(${PROJECT_NAME}
  Game
  Scene
)

set (CMAKE_CXX_FLAGS "-F/Library/Frameworks -framework SDL2_image -framework SDL2 -std=c++20")