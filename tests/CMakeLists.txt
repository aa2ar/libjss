include(FindBoost)

find_package(Boost COMPONENTS REQUIRED system unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

add_definitions("-DBOOST_TEST_DYN_LINK")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(jss_tests jss_tests.cpp)

target_link_libraries(jss_tests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_link_libraries(jss_tests libjss)

add_test(NAME jss_tests COMMAND jss_tests WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
