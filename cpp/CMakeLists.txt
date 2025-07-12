cmake_minimum_required(VERSION 3.31)
project(cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Compiler-specific flags
if(MSVC)
    # Add MSVC-specific C++17 flag and other options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /arch:AVX2")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    endif()
else()
    if(MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=haswell -mtune=haswell")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -msse4.1")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    endif()
endif()

# Create a library for each implementation
add_library(impl_simple STATIC pandigital_simple.cpp)  # Add this line
add_library(impl_base_simd STATIC pandigital_simd.cpp)
add_library(impl_avx2 STATIC pandigital_avx2.cpp)

# AVX-512 specific settings
add_library(impl_avx512 STATIC pandigital_avx512.cpp)
if(MSVC)
    target_compile_options(impl_avx512 PRIVATE "/arch:AVX512")
else()
    target_compile_options(impl_avx512 PRIVATE "-mavx512f" "-mavx512dq" "-mavx512bw" "-mavx512vl")
endif()

# AVX2 Advanced implementation
add_library(impl_avx2_advanced STATIC pandigital_avx2_advanced.cpp)
if(MSVC)
    target_compile_options(impl_avx2_advanced PRIVATE "/arch:AVX2")
    # Add AVX-512 support if available
    if(CMAKE_CXX_FLAGS MATCHES "/arch:AVX512")
        target_compile_definitions(impl_avx2_advanced PRIVATE "__AVX512F__")
        target_compile_options(impl_avx2_advanced PRIVATE "/arch:AVX512")
    endif()
else()
    target_compile_options(impl_avx2_advanced PRIVATE "-mavx2" "-mfma")
    # Add AVX-512 support if available
    if(CMAKE_CXX_FLAGS MATCHES "-mavx512")
        target_compile_definitions(impl_avx2_advanced PRIVATE "__AVX512F__")
        target_compile_options(impl_avx2_advanced PRIVATE "-mavx512f" "-mavx512dq" "-mavx512bw" "-mavx512vl")
    endif()
endif()

# Create the main executable
add_executable(pandigital main.cpp)
target_link_libraries(pandigital
    impl_simple           # Add this line
    impl_base_simd
    impl_avx2
    impl_avx512
    impl_avx2_advanced
)