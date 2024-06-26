cmake_minimum_required(VERSION 3.27)
project(prac-polynom-operator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -std=c++20")

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


add_subdirectory(core)

add_executable(${PROJECT_NAME}
        main.cpp
        src/table_drawer.cpp
        src/window.cpp
        src/button.cpp
        src/center_positioned_string.cpp
        src/general_data.cpp
        src/window_manager.cpp
        app/colors.h
        src/colors.cpp
        src/inoutput_field.cpp
        src/table_drawer.cpp
        src/rounded_rectangle.cpp
        drawers/table_header_drawer_interface.h
        drawers/table_drawer.h
        drawers/table_header_drawers.h
        app/tabs_drawer.h
        src/tabs_drawer.cpp

)

target_link_libraries(${PROJECT_NAME} PRIVATE polynom)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
