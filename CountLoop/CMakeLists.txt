cmake_minimum_required(VERSION 3.7)

project(CountLoop_Pass)

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_subdirectory(LLVMPassCountLoop)
