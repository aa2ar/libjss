set(LIB_PREFIX "")
set(LIBJSS_TARGET_NAME ${LIB_PREFIX}${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-fPIC")

file(GLOB_RECURSE SRC "*.cpp")

include_directories("${CMAKE_SOURCE_DIR}/include")

add_library(${LIBJSS_TARGET_NAME} SHARED ${SRC})
add_library(${LIBJSS_TARGET_NAME}-static STATIC ${SRC})
set_target_properties(${LIBJSS_TARGET_NAME} PROPERTIES VERSION "1.0")
set_target_properties(${LIBJSS_TARGET_NAME}-static PROPERTIES OUTPUT_NAME ${LIBJSS_TARGET_NAME})

# install
file(GLOB hdr_files ${CMAKE_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}/*.*)
install(TARGETS ${LIBJSS_TARGET_NAME} ${LIBJSS_TARGET_NAME}-static DESTINATION lib COMPONENT main)
install(FILES ${hdr_files} DESTINATION /usr/local/include/${CMAKE_PROJECT_NAME} COMPONENT dev)

# generate package
set(PKG_DEPENDS "")
set(PKG_VENDOR "aa2ar")
set(PKG_SUMMARY "Basic json serializer for C++")
set(PKG_DESCRIPTION "Basic json serializer for C++")
set(PKG_CONTACT "Alexander Arefyev, alex.arefyev@gmail.com")
include(${CMAKE_SOURCE_DIR}/cmake/mkpackage.cmake)
