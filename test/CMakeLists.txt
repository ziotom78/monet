add_executable(test_main "src/test-svg.cpp")

target_compile_features(test_main PUBLIC cxx_std_11)
target_link_libraries(test_main ziotom78_monet)
set(CMAKE_CXX_FLAGS "-Wall --coverage -g -O0 -fprofile-arcs -ftest-coverage")

