cmake_minimum_required(VERSION 3.16)
project(SimpleLangCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader executionengine interpreter mcjit native)

add_executable(simplelang 
    src/main.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/AST.cpp
    src/CodeGen.cpp
)

target_link_libraries(simplelang ${llvm_libs})