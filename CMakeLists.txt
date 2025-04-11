cmake_minimum_required(VERSION 3.30)

project(TinyApplication VERSION 1.0.0)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Validate build type
set(ALLOWED_BUILD_TYPES Debug Release RelWithDebInfo MinSizeRel)
if(NOT CMAKE_BUILD_TYPE IN_LIST ALLOWED_BUILD_TYPES)
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Useful for IDE integration and static analysis tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Check for C++23 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++23" COMPILER_SUPPORTS_CXX23)
if(NOT COMPILER_SUPPORTS_CXX23)
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++23 support.")
endif()

# Define source and include directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Download CPM.cmake if not present
set(CPM_DOWNLOAD_VERSION 0.27.2)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

# Find required packages
CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.15.0)

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Catch2 3 REQUIRED)

# MacOS universal binary setup
if(APPLE)
    execute_process(
            COMMAND uname -m
            OUTPUT_VARIABLE HOST_ARCHITECTURE
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(HOST_ARCHITECTURE STREQUAL "arm64")
        set(CMAKE_OSX_ARCHITECTURES "arm64")
    else()
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    endif()
    message(STATUS "osx arch: ${CMAKE_OSX_ARCHITECTURES}")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    message(STATUS "linux")
endif()

# Define dependent libraries
set(DEPENDENT_LIBRARIES
        spdlog::spdlog
        Catch2::Catch2WithMain
        pthread
        nlohmann_json::nlohmann_json
        domainkeys
        quickkv
)

# Add executables
add_executable(tiny-app ${SRC_DIR}/main.cpp)
add_executable(unit_tests tests/main_unit.cpp)

# Function to set up target properties
function(setup_target_properties target)
    target_include_directories(${target} PRIVATE
            ${INCLUDE_DIR}
            ${INCLUDE_DIR}/vendor
    )
    target_link_libraries(${target} PRIVATE ${DEPENDENT_LIBRARIES})

    # Set compiler options
    target_compile_options(${target} PRIVATE
            -Wall
            -Wpedantic
            -Wextra
            -Werror
    )

    # Set optimization flags based on build type
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${target} PRIVATE -O3)  # Adjust as needed
    else()
        target_compile_options(${target} PRIVATE -O0)  # Adjust as needed
    endif()
endfunction()

# Use the function for each target
foreach(target tiny-app unit_tests)
    setup_target_properties(${target})
endforeach()

# Enable testing
enable_testing()
add_test(unit_tests COMMAND unit_tests)

# Print compiler information
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
