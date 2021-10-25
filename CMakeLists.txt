cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 11)

project(ziotom78_monet VERSION 0.1.0 LANGUAGES CXX)

option(MONET_UseCairo "Use the Cairo library" ON)

set(ZIOTOM78_MONET_TARGET_NAME ${PROJECT_NAME})
set(ZIOTOM78_MONET_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include/")

add_library(${ZIOTOM78_MONET_TARGET_NAME} INTERFACE)

set(MONET_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")

target_sources(${ZIOTOM78_MONET_TARGET_NAME}
  INTERFACE "${MONET_INCLUDE_PATH}/monet.h")
target_include_directories(${ZIOTOM78_MONET_TARGET_NAME}
  INTERFACE ${ZIOTOM78_MONET_INCLUDE_BUILD_DIR})

add_executable(simple examples/simple.cpp)
target_include_directories(simple
  PUBLIC ${MONET_INCLUDE_PATH})

add_executable(complex examples/complex.cpp)
target_include_directories(complex
  PUBLIC ${MONET_INCLUDE_PATH})

add_executable(spirograph examples/spirograph.cpp)
target_include_directories(spirograph
  PUBLIC ${MONET_INCLUDE_PATH})

include(CTest)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
