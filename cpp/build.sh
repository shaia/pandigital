#!/bin/bash

# Default build type (empty means standard Linux build)
BUILD_TYPE=""

# Check if argument is provided
if [ $# -eq 1 ]; then
    BUILD_TYPE=$1
fi

# Create build directory with specific name based on build type
if [ "$BUILD_TYPE" == "debug" ]; then
    echo "Building debug version..."
    BUILD_DIR="build_debug"
    BUILD_CONFIG="-DCMAKE_BUILD_TYPE=Debug"
else
    echo "Building release version..."
    BUILD_DIR="build_release"
    BUILD_CONFIG="-DCMAKE_BUILD_TYPE=Release"
fi

# Create build directory if it doesn't exist
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Run CMake and build
cmake $BUILD_CONFIG ..
cmake --build .

# Return to original directory
cd ..

echo
echo "Build complete!"
echo "Executable location: $BUILD_DIR/cpp"
