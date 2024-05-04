cmake_minimum_required(VERSION 3.14)

project(Common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(Common SHARED
  CommonExport.h
  Enums.h
  Utils.cpp Utils.h
  Categories.h
  Accounts.h
)

target_compile_definitions(Common PRIVATE COMMON_LIBRARY)
