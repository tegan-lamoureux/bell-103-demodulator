cmake_minimum_required(VERSION 3.2.0)

project(Bell103Demod C CXX)

set(target_name "bell_103_demodulator")

# Target Names
set(app "${target_name}")
set(tests "${target_name}_unit_tests")


#################
#### Source Files

set(
    app_sources

    # Headers
    src/goertzel_filter.h

    # Sources
    src/goertzel_filter.cpp
)

set(
    tests_sources

    ${app_sources}

    tests/filter_tests.cpp
    tests/libsndfile_tests.cpp
)


##############
#### Libraries

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external_libraries")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/external_libraries")

find_package(LibSndFile REQUIRED)
if (LIBSNDFILE_FOUND)
    include_directories(${LIBSNDFILE_INCLUDE_DIR})
endif()


######################
#### Google Test Setup
#
# Downloads GTest from Google at configure time, and builds it at
# project compile time in the build directory. 

include(GoogleTest)


###################
#### Build Main App

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ${app}

    src/main.cpp
    ${app_sources}
)

#target_include_directories(
#    ${app} PUBLIC
#)

target_link_libraries(
    ${app}

    ${LIBSNDFILE_LIBRARIES}
)


################
#### Build Tests

add_executable(
    ${tests}

    ${tests_sources}
)

target_include_directories(
    ${tests} PUBLIC

    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    ${tests}

    gtest_main
    ${LIBSNDFILE_LIBRARIES}
)

