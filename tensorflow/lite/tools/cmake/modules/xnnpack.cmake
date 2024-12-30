#
# Copyright 2020 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if(TARGET xnnpack OR xnnpack_POPULATED)
  return()
endif()

include(OverridableFetchContent)

OverridableFetchContent_Declare(
  cpuinfo-xnnpack
  URL "${TENSORFLOW_SOURCE_DIR}/externals/XNNPACK-cpuinfo.tar.gz"
  PREFIX "${CMAKE_BINARY_DIR}"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/cpuinfo-xnnpack"
)
OverridableFetchContent_GetProperties(cpuinfo-xnnpack)
if(NOT cpuinfo-xnnpack_POPULATED)
  OverridableFetchContent_Populate(cpuinfo-xnnpack)
endif()
set(CPUINFO_SOURCE_DIR "${CMAKE_BINARY_DIR}/cpuinfo-xnnpack")

OverridableFetchContent_Declare(
  fp16
  URL "${TENSORFLOW_SOURCE_DIR}/externals/XNNPACK-fp16.tar.gz"
  PREFIX "${CMAKE_BINARY_DIR}"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/fp16"
)
OverridableFetchContent_GetProperties(fp16)
if(NOT fp16_POPULATED)
  OverridableFetchContent_Populate(fp16)
endif()
set(FP16_SOURCE_DIR "${CMAKE_BINARY_DIR}/fp16")

OverridableFetchContent_Declare(
  psimd
  URL "${TENSORFLOW_SOURCE_DIR}/externals/XNNPACK-FP16-psimd.tar.gz"
  PREFIX "${CMAKE_BINARY_DIR}"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/psimd"
)
OverridableFetchContent_GetProperties(psimd)
if(NOT psimd_POPULATED)
  OverridableFetchContent_Populate(psimd)
endif()
set(PSIMD_SOURCE_DIR "${CMAKE_BINARY_DIR}/psimd")

OverridableFetchContent_Declare(
  fxdiv
  URL "${TENSORFLOW_SOURCE_DIR}/externals/XNNPACK-fxdiv.tar.gz"
  PREFIX "${CMAKE_BINARY_DIR}"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/fxdiv"
)
OverridableFetchContent_GetProperties(fxdiv)
if(NOT fxdiv_POPULATED)
  OverridableFetchContent_Populate(fxdiv)
endif()
set(FXDIV_SOURCE_DIR "${CMAKE_BINARY_DIR}/fxdiv")

OverridableFetchContent_Declare(
  pthreadpool
  URL "${TENSORFLOW_SOURCE_DIR}/externals/XNNPACK-pthreadpool.tar.gz"
  PREFIX "${CMAKE_BINARY_DIR}"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/pthreadpool-xnnpack"
)
OverridableFetchContent_GetProperties(pthreadpool)
if(NOT pthreadpool_POPULATED)
  OverridableFetchContent_Populate(pthreadpool)
endif()
set(PTHREADPOOL_SOURCE_DIR "${CMAKE_BINARY_DIR}/pthreadpool-xnnpack")

OverridableFetchContent_Declare(
  kleidiai
  URL "${TENSORFLOW_SOURCE_DIR}/externals/XNNPACK-kleidiai.tar.gz"
  PREFIX "${CMAKE_BINARY_DIR}"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/kleidiai-xnnpack"
  CMAKE_ARGS -DKLEIDIAI_BUILD_TESTS=OFF
)

OverridableFetchContent_GetProperties(kleidiai)
if(NOT kleidiai_POPULATED)
  OverridableFetchContent_Populate(kleidiai)
endif()
set(KLEIDIAI_SOURCE_DIR "${CMAKE_BINARY_DIR}/kleidiai-xnnpack")

# Disable building kleidiai test by modifying the cmake file.
execute_process(COMMAND sed -i "s|include(FetchGTest)|#include(FetchGTest)|" ${CMAKE_BINARY_DIR}/kleidiai-xnnpack/CMakeLists.txt)
execute_process(COMMAND sed -i "s|option(KLEIDIAI_BUILD_TESTS       \"Build unit tests.\"             ON)|option(KLEIDIAI_BUILD_TESTS       \"Build unit tests.\"             OFF)|" ${CMAKE_BINARY_DIR}/kleidiai-xnnpack/CMakeLists.txt)

OverridableFetchContent_Declare(
  xnnpack
  URL "${TENSORFLOW_SOURCE_DIR}/externals/xnnpack.tar.gz"
  # GIT_REPOSITORY https://github.com/google/XNNPACK
  # # Sync with tensorflow/workspace2.bzl
  # GIT_TAG 6b83f69d4938da4dc9ad63c00bd13e9695659a51
  # GIT_PROGRESS TRUE
  PREFIX "${CMAKE_BINARY_DIR}"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/xnnpack"
)
OverridableFetchContent_GetProperties(xnnpack)
if(NOT xnnpack_POPULATED)
  OverridableFetchContent_Populate(xnnpack)
endif()

# May consider setting XNNPACK_USE_SYSTEM_LIBS if we want to control all
# dependencies by TFLite.
set(XNNPACK_BUILD_TESTS OFF CACHE BOOL "Disable XNNPACK test.")
set(XNNPACK_BUILD_BENCHMARKS OFF CACHE BOOL "Disable XNNPACK benchmarks.")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types")

# The following line adds project of PTHREADPOOL, FP16 and XNNPACK which are
# needed to compile XNNPACK delegate of TFLite.
# Note, we introduce an intermediate subdirectory, see ${TFLITE_SOURCE_DIR}/tools/cmake/modules/xnnpack/CMakeLists.txt
# for details.
add_subdirectory(${TFLITE_SOURCE_DIR}/tools/cmake/modules/xnnpack)

include_directories(
  AFTER
   "${PTHREADPOOL_SOURCE_DIR}/include"
   "${FP16_SOURCE_DIR}/include"
   "${FXDIV_SOURCE_DIR}/include"
   "${KLEIDIAI_SOURCE_DIR}/"
   "${CPUINFO_SOURCE_DIR}/"
   "${XNNPACK_SOURCE_DIR}/include"
)
