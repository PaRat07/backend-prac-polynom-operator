cmake_minimum_required(VERSION 3.22)
project(prac-polynom-operator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC")


include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


set(CORE
        src/single_linked_list.cpp
)

set(COMPONENTS
        src/table.cpp
        src/button.cpp
        src/inoutput_field.cpp
        elements/fab.h
)

set(SHAPES
        src/center_positioned_string.cpp
        src/rounded_rectangle.cpp
)

set(APP
        src/window.cpp
        src/general_data.cpp
        src/window_manager.cpp
        app/colors.h
        src/colors.cpp
)

add_executable(${PROJECT_NAME}
        main.cpp
        ${COMPONENTS}
        ${SHAPES}
        ${APP}
)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)