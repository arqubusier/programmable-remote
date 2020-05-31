cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

project(remote_test)
set(GOOGLETEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../googletest")

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory("${GOOGLETEST_DIR}/googletest"
                 "${GOOGLETEST_DIR}/googlemock")

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test test.cxx)
target_link_libraries(test gtest_main)
add_test(NAME example_test COMMAND example)
