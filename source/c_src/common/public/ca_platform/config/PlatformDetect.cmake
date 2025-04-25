message(STATUS "==== Detecting platform... ====")

# ========== OS ==========
if(WIN32)
    set(CA_OS_WINDOWS 1)
elseif(APPLE)
    set(CA_OS_MACOS 1)
elseif(UNIX)
    set(CA_OS_LINUX 1)
else()
    message(WARNING "Unknown OS!")
endif()

# ========== Compiler ==========
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CA_COMPILER_CLANG 1)
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(CA_COMPILER_GCC 1)
elseif(MSVC)
    set(CA_COMPILER_MSVC 1)
else()
    message(WARNING "Unknown compiler!")
endif()

# ========== Architecture ==========
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" DETECTED_CPU_ARCH)
message(STATUS "Normalized CPU architecture: ${DETECTED_CPU_ARCH}")

if(DETECTED_CPU_ARCH MATCHES "^(x86_64|amd64|x64)$")
    set(CA_ARCH_X86_64 1)
    message(STATUS "Detected architecture: x86_64")
elseif(DETECTED_CPU_ARCH MATCHES "^(i386|i686|x86)$")
    set(CA_ARCH_X86_32 1)
    message(STATUS "Detected architecture: x86 (32-bit)")
elseif(DETECTED_CPU_ARCH MATCHES "^(aarch64|arm64|armv8|arm64e)$")
    set(CA_ARCH_ARM64 1)
    message(STATUS "Detected architecture: ARM64 (AArch64)")
elseif(DETECTED_CPU_ARCH MATCHES "^(armv7l|armv7|arm)$")
    set(CA_ARCH_ARM32 1)
    message(STATUS "Detected architecture: ARM 32-bit")
elseif(DETECTED_CPU_ARCH MATCHES "^(riscv64)$")
    set(CA_ARCH_RISCV64 1)
    message(STATUS "Detected architecture: RISC-V 64-bit")
elseif(DETECTED_CPU_ARCH MATCHES "^(ppc64|powerpc64)$")
    set(CA_ARCH_PPC64 1)
    message(STATUS "Detected architecture: PowerPC 64-bit")
else()
    message(WARNING "⚠️ Unknown architecture detected: '${CMAKE_SYSTEM_PROCESSOR}' (normalized: '${DETECTED_CPU_ARCH}')")
    set(CA_ARCH_UNKNOWN 1)
endif()

# ========== SIMD ==========
include(CheckCCompilerFlag)
check_c_compiler_flag("-msse2" CA_HAVE_SSE2)
check_c_compiler_flag("-mavx" CA_HAVE_AVX)
check_c_compiler_flag("-mfpu=neon" CA_HAVE_NEON)

# ========== OpenMP ==========
find_package(OpenMP)
if(OpenMP_C_FOUND)
    set(CA_HAS_OPENMP 1)
endif()

# ========== CUDA ==========
find_package(CUDAToolkit)
if(CUDAToolkit_FOUND)
    message(STATUS "CUDA Toolkit found")
    set(CA_HAVE_CUDA 1)
endif()

# ========== OpenCL ==========
find_path(OPENCL_INCLUDE_DIR CL/cl.h)
find_library(OPENCL_LIBRARY NAMES OpenCL)
if(OPENCL_INCLUDE_DIR AND OPENCL_LIBRARY)
    set(CA_HAVE_OPENCL 1)
endif()

# ========== ROCm / HIP ==========
find_path(HIP_INCLUDE_DIR hip/hip_runtime.h)
if(HIP_INCLUDE_DIR)
    set(CA_HAVE_ROCM 1)
endif()