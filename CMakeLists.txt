cmake_minimum_required(VERSION 4.0.0 FATAL_ERROR)
project("Azalealang" LANGUAGES C VERSION 1.0.0.14 DESCRIPTION "A small C-like language built for use in the Cobalt operating system.")

if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "${PROJECT_NAME} does not support this operating system.")
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
    message(WARNING "No build type provided, assuming debug.")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_definitions(VERSION_STRING="${PROJECT_VERSION}")
add_compile_options(-Wall -Wextra -Wpedantic -Werror -nostdlib -fno-stack-protector)
add_link_options(-nostdlib -fno-stack-protector)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_compile_options(-g0)
endif()

include_directories("${CMAKE_SOURCE_DIR}/Include")

string(FIND ${PROJECT_NAME} "lang" PROJECT_REALNAME_INDEX)
string(SUBSTRING ${PROJECT_NAME} 0 ${PROJECT_REALNAME_INDEX} PROJECT_REALNAME)

add_library(${PROJECT_REALNAME} "${CMAKE_SOURCE_DIR}/Source/Compiler.c" "${CMAKE_SOURCE_DIR}/Source/Utilities.c" "${CMAKE_SOURCE_DIR}/Source/Targets/Linux.c")
add_executable(${PROJECT_REALNAME}C "${CMAKE_SOURCE_DIR}/Source/Client.c") 

add_dependencies(${PROJECT_REALNAME}C ${PROJECT_REALNAME})
target_link_libraries(${PROJECT_REALNAME}C ${PROJECT_REALNAME})

