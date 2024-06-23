cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT TARGET bave)
  include(FetchContent)

  FetchContent_Declare(
    bave
    GIT_REPOSITORY https://github.com/karnkaul/bave
    GIT_TAG v0.5.6
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/bave"
  )

  FetchContent_MakeAvailable(bave)
endif()

project(bgf VERSION 0.1.6)

add_subdirectory(lib)
