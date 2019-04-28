cmake_minimum_required(VERSION 3.2.0)

project(Bell103Demod C CXX)

set(target_name "demodulator")

# Target Names
set(app "${target_name}")
set(tests "${target_name}_unit_tests")


#################
#### Source Files

set(
    app_sources

    # Headers
    

    # Sources
    src/main.cpp
)

set(
    tests_sources

    tests/demod_tests.cpp
)


##############
#### Libraries

# libwav located locally as a submodule
#add_subdirectory("${CMAKE_SOURCE_DIR}/external_libraries/libwav/")


######################
#### Google Test Setup
#
# Downloads GTest from Google at configure time, and builds it at
# project compile time in the build directory. 

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/tests")
include(GoogleTest)


###################
#### Build Main App

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ${app}

    ${app_sources}
)

target_include_directories(
	${app} PUBLIC

#	${CMAKE_SOURCE_DIR}/external_libraries/libwav/src/include
)

target_link_libraries(
    ${app}

#	wav #libwav cmake project name
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
#	${CMAKE_SOURCE_DIR}/external_libraries/libwav/src/include
)

target_link_libraries(
    ${tests}

    gtest_main
#	wav #libwav cmake project name
)

