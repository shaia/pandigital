# Pandigital Multiples: Multi‑Language Implementations

This repository collects implementations of the Project Euler **Problem 38** (“Pandigital multiples”)—finding the largest 1–9 pandigital 9‑digit number formed as the concatenated product of an integer with (1,2,…,n), n>1—in several programming languages and optimization styles.

---

## 🔍 Problem Overview

Given an integer `k`, form the concatenated product:

```
k × 1  ‖  k × 2  ‖  ...  ‖  k × n
```

so that the result is exactly 9 digits and contains each digit 1–9 exactly once.  Find the maximum such 9‑digit pandigital number.

The known answer is **932718654**, arising from `k = 9327`, `n = 2`.

---

## 📂 Repository Structure

```
/                # repo root
  ├─ cpp/         # C++ implementations
  │    ├─ simple/             # brute‑force version
  │    ├─ simd/               # AVX‑SIMD optimized version
  │    └─ cuda/               # CUDA/GPU version
  ├─ ispc/        # ISPC + C++ AVX‑512 implementation
  ├─ matlab/      # MATLAB scripts
  │    ├─ optimized_pandigital_fixed.m
  │    └─ pandigital_vectorized_full.m
  ├─ mathematica/ # Wolfram Mathematica notebook/script
  └─ README.md    # this file
```

---

## 🛠️ How to Build & Run

### C++ (Brute‑force)

```bash
g++ cpp/simple/SimplePandigital.cpp -std=c++11 -O2 -o simple_pandigital
./simple_pandigital
```

### C++ SIMD (AVX‑512)

```bash
# Requires AVX‑512 support
g++ cpp/simd/PandigitalAVX512.cpp -O3 -mavx512f -mfma -o simd_pandigital
./simd_pandigital
```

### CUDA (GPU)

```bash
nvcc cuda/PandigitalGPU.cu -O2 -o gpu_pandigital
./gpu_pandigital
```

### ISPC + C++

```bash
# Using ISPC
ispc ispc/pandigital.ispc --target=avx512 --arch=x86-64 -O2 -o ispc_pandigital.o -h pandigital_ispc.h
g++ cpp/ispc_main.cpp ispc_pandigital.o -O2 -mavx512f -o ispc_pandigital
./ispc_pandigital
```

### MATLAB

Open MATLAB and run:

```matlab
run('matlab/optimized_pandigital_fixed.m')
run('matlab/pandigital_vectorized_full.m')
```

### Mathematica

Open the provided `.nb` or run the script:

```wolfram
<< "mathematica/pandigital.nb"
```

---

## 🚀 Performance Highlights

* **Brute‑force C++**: straightforward, easy to understand (\~ms runtime).
* **SIMD C++**: AVX2/AVX‑512 fused multiply‑add, branch‑free digit masking (\~µs–ms runtime).
* **CUDA**: parallel threads on GPU handle all `k` in one kernel launch.
* **ISPC**: SPMD language for portable SIMD across AVX‑512.
* **MATLAB**: fully vectorized, no loops for core filtering.
* **Mathematica**: concise table driven approach.

---

## 📖 Reference

* Project Euler Problem 38: [https://projecteuler.net/problem=38](https://projecteuler.net/problem=38)

---

Happy exploring and benchmarking!
