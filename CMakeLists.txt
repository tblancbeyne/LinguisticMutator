cmake_minimum_required (VERSION 2.8)
project (LinguisticMutator)

# Options de compilateur
list(APPEND CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS} -g -Wall")

# Mettre les binaires dans bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Chercher les headers dans le dossier include
include_directories(include)

# Project version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

add_subdirectory(src)
