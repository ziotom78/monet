set(CMAKE_CXX_FLAGS "-Wall --coverage -g -O0 -fprofile-arcs -ftest-coverage")

enable_testing()

function(add_monet_test target)
  add_executable(${target} ${ARGN})
  target_compile_features(${target} PUBLIC cxx_std_11)
  target_link_libraries(${target} ziotom78_monet)

  add_test(${target} ${target})
endfunction()

add_monet_test(test-svg "src/test-svg.cpp")
add_monet_test(test-transforms "src/test-transforms.cpp")
  
