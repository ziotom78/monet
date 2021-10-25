cmake_minimum_required(VERSION 3.10)

project(ziotom78_monet VERSION 0.0.1 LANGUAGES CXX)

option(MONET_UseCairo "Use the Cairo library" ON)

set(ZIOTOM78_MONET_TARGET_NAME ${PROJECT_NAME})
set(ZIOTOM78_MONET_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include/")

add_library(${ZIOTOM78_MONET_TARGET_NAME} INTERFACE)
target_sources(${ZIOTOM78_MONET_TARGET_NAME}
  INTERFACE "${PROJECT_SOURCE_DIR}/include/monet.h")
target_include_directories(${ZIOTOM78_MONET_TARGET_NAME}
  INTERFACE ${ZIOTOM78_MONET_INCLUDE_BUILD_DIR})
target_compile_features(${ZIOTOM78_MONET_TARGET_NAME} INTERFACE cxx_std_11)

include(CTest)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
