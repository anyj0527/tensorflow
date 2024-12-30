# #
# # Copyright 2022 The TensorFlow Authors. All Rights Reserved.
# #
# # Licensed under the Apache License, Version 2.0 (the "License");
# # you may not use this file except in compliance with the License.
# # You may obtain a copy of the License at
# #
# #      https://www.apache.org/licenses/LICENSE-2.0
# #
# # Unless required by applicable law or agreed to in writing, software
# # distributed under the License is distributed on an "AS IS" BASIS,
# # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# # See the License for the specific language governing permissions and
# # limitations under the License.

# CMAKE_MINIMUM_REQUIRED(VERSION 3.5 FATAL_ERROR)

# PROJECT(pthreadpool-download NONE)

# INCLUDE(ExternalProject)

# ExternalProject_Add(fxdiv
#   URL "${TENSORFLOW_SOURCE_DIR}/externals/XNNPACK-fxdiv.tar.gz"
#   CMAKE_ARGS -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_BINARY_DIR}/FXdiv
#              -DFXDIV_BUILD_TESTS=OFF
#              -DFXDIV_BUILD_BENCHMARKS=OFF
#   PREFIX "${CMAKE_BINARY_DIR}"
#   SOURCE_DIR "${CMAKE_BINARY_DIR}/FXdiv-source"
#   BINARY_DIR "${CMAKE_BINARY_DIR}/FXdiv"
# )
# set(FXDIV_SOURCE_DIR "${CMAKE_BINARY_DIR}/FXdiv-source" CACHE STRING "fxdiv source dir for pthreadpool")
# add_definitions(-DFXDIV_SOURCE_DIR=${FXDIV_SOURCE_DIR})
# set(PTHREADPOOL_BUILD_TESTS OFF CACHE BOOL "Disable test build for pthreadpool")
# set(PTHREADPOOL_BUILD_BENCHMARKS OFF CACHE BOOL "Disable benchmarks build for pthreadpool")
# ExternalProject_Add(pthreadpool
#   # URL https://github.com/Maratyszcza/pthreadpool/archive/4fe0e1e183925bf8cfa6aae24237e724a96479b8.zip
#   URL "${TENSORFLOW_SOURCE_DIR}/externals/XNNPACK-pthreadpool.tar.gz"
#   # URL_HASH SHA256=a4cf06de57bfdf8d7b537c61f1c3071bce74e57524fe053e0bbd2332feca7f95
#   CMAKE_ARGS "-DFXDIV_SOURCE_DIR=${CMAKE_BINARY_DIR}/FXdiv-source"
#              "-DPTHREADPOOL_BUILD_TESTS=OFF"
#              "-DPTHREADPOOL_BUILD_BENCHMARKS=OFF"
#              "-DCMAKE_VERBOSE_MAKEFILE=ON"
#   PREFIX "${CMAKE_BINARY_DIR}"
#   SOURCE_DIR "${CMAKE_BINARY_DIR}/pthreadpool-source"
#   BINARY_DIR "${CMAKE_BINARY_DIR}/pthreadpool"
#   CONFIGURE_COMMAND ""
#   BUILD_COMMAND ""
#   INSTALL_COMMAND ""
#   TEST_COMMAND ""
# )
