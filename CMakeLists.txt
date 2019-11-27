# cmake_minimum_required(VERSION <specify CMake version here>)
project(AVLTree)

set(CMAKE_CXX_STANDARD 14)

add_executable(AVLTree main.cpp consolecolor.h consolecolor.cpp AppBase.h Log.h Context.h Menu.h listener.h App.h App.cpp bnode.h AVLTree.h Tool.h LoginData.h Tool.cpp)