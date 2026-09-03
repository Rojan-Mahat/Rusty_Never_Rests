cmake_minimum_required(VERSION 3.20)
project (DinoRunner LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DinoRunner src/main.cpp
                          src/player.cpp
                          src/game.cpp
                          src/enemy.cpp
                          src/fireball.cpp)

find_package(SFML 3 COMPONENTS Graphics REQUIRED)
target_link_libraries(DinoRunner PRIVATE SFML::Graphics)
target_include_directories(DinoRunner PRIVATE include)