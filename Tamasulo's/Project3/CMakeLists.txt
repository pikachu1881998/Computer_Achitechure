cmake_minimum_required(VERSION 3.15)
project(Project3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project3
        traces/bwaves603_2M.trace
        traces/gcc602_2M.trace
        traces/mcf605_2M.trace
        traces/perlbench600_2M.trace
        Makefile
        procsim
        procsim.cpp
        procsim.h
        procsim_driver.cpp
        traces.tar.gz)
