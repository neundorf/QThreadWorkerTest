cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(CR)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

qt_wrap_cpp(mocs DoStuff.h
                 Command.h
                 CommandInCommand.h
                 CommandRunner.h
                 MainThreadRunner.h
            )

add_executable(cr main.cpp
                  DoStuff.cpp
                  Command.cpp
                  CommandInCommand.cpp
                  CommandRunner.cpp
                  MainThreadRunner.cpp
                  ${mocs})

target_link_libraries(cr Qt::Core fmt::fmt)
