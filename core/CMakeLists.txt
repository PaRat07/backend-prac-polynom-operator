cmake_minimum_required(VERSION 3.27)
project(polynom)

set(SRCS
        ../src/monomial.cpp
        ../src/polynomial.cpp
        ../src/parser.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
