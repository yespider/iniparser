cmake_minimum_required(VERSION 2.8)

include_directories(src)

add_executable(ini-touch src/ini_touch.cpp src/accumulover.cpp src/iniparser.c src/dictionary.c )