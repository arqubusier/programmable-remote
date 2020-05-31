cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(remote_test)
set(GOOGLETEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../googletest")

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory("${GOOGLETEST_DIR}/googletest"
                 "${GOOGLETEST_DIR}/googlemock")

add_executable(test test.cxx)
target_include_directories(test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../src")
target_include_directories(test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../etl/include")
target_link_libraries(test gtest_main)
