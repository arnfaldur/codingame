cmake_minimum_required(VERSION 3.10)
project(Legends_of_Code_and_Magic)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXE_LINKER_FLAGS -lpthread)

add_executable(Legends_of_Code_and_Magic player.cpp )