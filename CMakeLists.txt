cmake_minimum_required(VERSION 3.20)
project (RustyNeverRests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RustyNeverRests src/main.cpp
                          src/player.cpp
                          src/game.cpp
                          src/enemy.cpp
                          src/fireball.cpp
                          src/powerup.cpp)

find_package(SFML 3 COMPONENTS Graphics REQUIRED)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)
target_link_libraries(RustyNeverRests PRIVATE SFML::Graphics)
target_link_libraries(RustyNeverRests PRIVATE SFML::Graphics SFML::Audio)
target_include_directories(RustyNeverRests PRIVATE include)