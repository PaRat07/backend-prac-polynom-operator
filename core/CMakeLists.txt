cmake_minimum_required(VERSION 3.27)
project(polynom)

set(SRCS
        ../src/single_linked_list.cpp
        ../src/monomial.cpp
        ../src/polynomial.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
