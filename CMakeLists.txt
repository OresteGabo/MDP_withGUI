cmake_minimum_required(VERSION 3.27)
project(MDP_GUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(MDP_GUI main.cpp
        Widget.cpp
        Widget.h
        Agent.cpp
        Agent.h
        Environment.cpp
        Environment.h
)
target_link_libraries(MDP_GUI
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

