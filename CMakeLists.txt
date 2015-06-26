cmake_minimum_required(VERSION 2.8.3 FATAL_ERROR)
project(mat_to_eigen_matrix)
# convert matlab data in .mat file to Eigen Matrix in C/C++
# Zekun 2015-06

# C++ 11 is needed for some special types
add_definitions(
  -std=c++11
)

# MATLAB C/C++ headers
include_directories(
  /usr/local/MATLAB/R2015a/extern/include
)

# MATLAB libraries
link_directories(
  lib
  /usr/local/lib
  /usr/lib
  /usr/local/MATLAB/R2015a/bin/glnxa64
  /usr/local/MATLAB/R2015a/extern/lib
  /usr/local/MATLAB/R2015a/sys/os/glnxa64
)

add_library(mat_to_eigen SHARED src/mat_to_eigen.cpp)
target_link_libraries(
  mat_to_eigen
  mat mx mwfl mwi18n
  stdc++ m
)

add_executable(test src/test.cpp)
target_link_libraries(
  test
  mat_to_eigen
  mat mx mwfl mwi18n
  stdc++ m
)

