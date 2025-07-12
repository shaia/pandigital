# Pandigital Number Calculator

## Overview
This project implements various optimized algorithms to find pandigital numbers. It compares different implementation approaches:
- Simple sequential implementation
- Base SIMD optimization
- AVX2-optimized implementation
- AVX-512-optimized implementation (when supported)

Each implementation aims to find the largest 9-digit pandigital number formed by concatenating k and 2k for some integer k.

## System Requirements

### Minimum Requirements
- CMake 3.31+
- C++17 compatible compiler
- CPU supporting SSE4.1 and AVX2

### Optional Requirements
- CPU with AVX-512 support for the AVX-512 implementation

### Supported Platforms & Compilers
- Windows:
  - Visual Studio 2022
  - MinGW-w64
- Linux:
  - GCC 7+
  - Clang 5+

## Building

### Windows

1. Using Visual Studio 2022:
```batch 
build.bat
```

Output: `build_vs\Release\pandigital.exe`

2. Using MinGW:
```batch
build_mingw.bat
```

Output: `build_mingw\pandigital.exe`

### Linux/macOS
```bash 
./build.sh
```
1. Release build:
Output: `build_release/pandigital`

2. Debug build:
```bash 
./build.sh debug
```
Output: `build_debug/pandigital`

### Manual Build
```bash
 mkdir build cd build cmake .. cmake --build .
```
## Running

Execute the compiled binary. The program will:
1. Check CPU features
2. Run each available implementation
3. Display a comparison table with results:

The table shows:
- Implementation: Algorithm variant
- Max Value: Largest pandigital number found
- Best K: Value of k that produces this number
- Best N: Multiplier (always 2 in this implementation)
- Time: Average execution time in milliseconds
- Valid: Consistency check across iterations

## Implementation Details

### Simple Implementation
- Basic sequential algorithm
- No SIMD optimizations
- Used as baseline for performance comparison

### Base SIMD Implementation
- Uses basic SIMD operations
- Processes 8 numbers simultaneously
- Includes scalar fallback for validation

### AVX2 Implementation
- Uses AVX2 instructions
- Processes 8 32-bit integers in parallel
- Includes optimized digit counting and mask operations

### AVX-512 Implementation
- Available on supported CPUs
- Processes 16 32-bit integers in parallel
- Includes advanced mask operations

## Troubleshooting

### Common Issues

1. CMake Version Error
   ```
   CMake Error: CMake 3.31 or higher is required
   ```
   Solution: Update CMake or modify CMakeLists.txt minimum version

2. Compiler Support
   ```
   error: unknown architecture 'avx512'
   ```
   Solution: Use a newer compiler or disable AVX-512 support

3. Build Directory Issues
   ```
   The build directory already exists
   ```
   Solution: Delete the build directory and try again:
   ```bash
   rm -rf build_*
   ./build.sh
   ```

4. Windows-Specific
   ```
   'MSVC' is not recognized
   ```
   Solution: Run from Visual Studio Developer Command Prompt

### Performance Notes

- AVX-512 implementation requires CPU support
- Performance may vary based on CPU model and OS
- Debug builds will be significantly slower
- Run multiple iterations for accurate benchmarking

## License

This project is available under the MIT License. See LICENSE file for details.