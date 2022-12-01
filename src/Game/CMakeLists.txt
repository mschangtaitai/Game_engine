add_library(Game Game.cpp)

set (CMAKE_CXX_FLAGS "-F/Library/Frameworks -framework SDL2 -framework SDL2_image -std=c++20")