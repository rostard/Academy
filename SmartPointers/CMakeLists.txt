cmake_minimum_required(VERSION 3.13)
project(SmartPointers)

set(CMAKE_CXX_STANDARD 17)

add_executable(SmartPointers main.cpp Block.cpp Block.h SharedBase.cpp SharedBase.h SharedPtr.cpp SharedPtr.h WeakPtr.cpp WeakPtr.h)