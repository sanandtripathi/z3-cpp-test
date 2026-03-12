cmake_minimum_required(VERSION 3.14)
project(z3_practice CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(Z3_LIB z3 REQUIRED)
find_path(Z3_INCLUDE_DIR z3++.h REQUIRED)

include_directories(${Z3_INCLUDE_DIR})

set(QUESTIONS
    q1_linear_arithmetic
    q2_boolean_sat
    q3_sudoku
    q4_nqueens
    q5_optimization
    q6_bitvectors
)

foreach(Q ${QUESTIONS})
    add_executable(${Q} src/${Q}.cpp)
    target_link_libraries(${Q} ${Z3_LIB})
endforeach()
