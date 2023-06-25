cmake_minimum_required(VERSION 3.0)
project(test C)

set(CMAKE_C_STANDARD 11)

add_executable(test main.c allocTracker.h allocTracker.c wrapperFuncs.h wrapperFuncs.c runtimeTracker.h runtimeTracker.c instTracker.h instTracker.c instProf.h instProf.c)
