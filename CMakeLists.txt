cmake_minimum_required(VERSION 2.8)

project(libjss)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/deb")

add_subdirectory(src)
add_subdirectory(tests)
