cmake_minimum_required(VERSION 3.2)
project(Slide)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Bstatic -static-libstdc++ -static-libgcc -lpthread -Bdynamic")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES main.cpp)
add_executable(Slide ${SOURCE_FILES} Game.cpp Game.h Graphics.cpp Graphics.h Enemie.h)
target_link_libraries(Slide sfml-graphics sfml-window sfml-audio sfml-system thor)