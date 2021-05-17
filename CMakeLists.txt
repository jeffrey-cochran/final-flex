cmake_minimum_required(VERSION 3.10)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)

project(FinalFlex)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
set (CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

option(USE_ADDRESS_SANITIZER "Enable address sanitiezr when CMAKE_BUILD_TYPE is Debug" OFF)
option(USE_ADDRESS_SANITIZER_RELEASE "Enable address sanitiezr in Release mode" OFF)

if (USE_ADDRESS_SANITIZER)
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
	set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

if (USE_ADDRESS_SANITIZER_RELEASE)
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-omit-frame-pointer -fsanitize=address")
	set (CMAKE_LINKER_FLAGS_RELEASE "${CMAKE_LINKER_FLAGS_RELEASE} -fno-omit-frame-pointer -fsanitize=address")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fno-omit-frame-pointer -fsanitize=address")
	set (CMAKE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_LINKER_FLAGS_RELWITHDEBINFO} -fno-omit-frame-pointer -fsanitize=address")
endif()

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Common functions
include(cmake/common_functions.cmake)

#
# Local Libraries
# ===============
include_directories(${CMAKE_SOURCE_DIR}/lib/core/pbd)


#
# Core libraries
# ==============
# Threads
find_package(Threads REQUIRED)

# Eigen 3
find_package(Eigen3 REQUIRED NO_MODULE)

add_library(final_flex_dep INTERFACE)
target_link_libraries(final_flex_dep INTERFACE Eigen3::Eigen Threads::Threads)

# SFML
add_subdirectory(third-party/SFML/)

#
# Executable declaration
add_executable(final_flex main.cpp)

target_include_directories(final_flex BEFORE PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(final_flex PUBLIC "${PROJECT_BINARY_DIR}")
target_sources(final_flex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/particle.cpp)
target_sources(final_flex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/params.cpp)
target_sources(final_flex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/SphereBVH.cpp)
target_sources(final_flex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/StrainLink.cpp)
target_sources(final_flex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/utils.cpp)
target_sources(final_flex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/blob.cpp)
target_sources(final_flex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/scenario.cpp)
target_sources(final_flex PUBLIC ${CMAKE_CURRENT_LIST_DIR}/fixture.cpp)


#
# box2d
target_include_directories(final_flex PUBLIC $ENV{BOX2D_INCLUDE_DIR})
target_link_libraries(final_flex PRIVATE $ENV{BOX2D_LIB})

#
# SFML
target_link_libraries(final_flex PUBLIC sfml-graphics sfml-audio)

#
# Eigen / Threads
target_link_libraries(final_flex PUBLIC final_flex_dep)
