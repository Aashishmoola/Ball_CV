# ============================================================================
# CMake minimum version and project definition
# ============================================================================
# Require CMake 3.14+ for modern target-based features and improved
# find_package support.
cmake_minimum_required(VERSION 3.14)

# Define the project name, version, and language.
# "ball_cv" is the project name used throughout the build system.
project(ball_cv VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# C++ standard configuration
# ============================================================================
# Use C++17 for modern language features (structured bindings, std::optional, etc.)
set(CMAKE_CXX_STANDARD 17)

# Enforce the C++ standard — fail if the compiler doesn't support it,
# rather than silently falling back to an older standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions (e.g., GNU extensions) to ensure
# portable, standards-compliant code.
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Compile commands for IDE support (VS Code, CLion, etc.)
# ============================================================================
# Generate compile_commands.json in the build directory. VS Code's C/C++
# extension uses this file to resolve includes, defines, and compiler flags
# for IntelliSense — no manual includePath configuration needed.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# OpenCV configuration
# ============================================================================
# Tell CMake where to find OpenCV's CMake config files.
# Since OpenCV is built from source at ~/Libraries/opencv, we point to its
# build directory. If you've built OpenCV in a different location, update this.
set(OpenCV_DIR "/home/aashishrapsodo/Libraries/opencv/build")

# Locate the OpenCV package. REQUIRED means the build will fail with a clear
# error if OpenCV is not found, rather than silently continuing.
# This defines variables like OpenCV_INCLUDE_DIRS and OpenCV_LIBS, and also
# creates imported targets (e.g., opencv_core, opencv_imgproc).
find_package(OpenCV REQUIRED)

# Print the detected OpenCV version during configuration for verification.
message(STATUS "Found OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

# ============================================================================
# Collect source files
# ============================================================================
# Gather all .cpp source files from the src/ directory.
# Using a variable keeps the executable definition clean and easy to maintain.
set(SOURCES
    src/main.cpp
    src/ball_det/back_sub/back_sub.cpp
    src/ball_det/preproc/preproc.cpp
    src/ball_det/h_transform/h_transform.cpp
    src/video_ex/video_ex.cpp
    src/data_val_f/data_val_f.cpp
)

# ============================================================================
# Define the executable target
# ============================================================================
# Create the executable from the collected source files.
# "ball_cv" will be the name of the output binary.
add_executable(${PROJECT_NAME} ${SOURCES})

# ============================================================================
# Include directories
# ============================================================================
# Add OpenCV headers so #include <opencv2/...> resolves correctly.
# Using target_include_directories (instead of the global include_directories)
# scopes the includes to this target only — a CMake best practice.
target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

# ============================================================================
# Link libraries
# ============================================================================
# Link OpenCV libraries (core, imgproc, highgui, etc.) to our executable.
# OpenCV_LIBS contains all the libraries found by find_package(OpenCV).
# PRIVATE means these are only needed for building this target, not for
# consumers of this target (relevant if this were a library).
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
)

# ============================================================================
# Build type default
# ============================================================================
# If no build type is specified (common in single-config generators like
# Makefiles), default to Release for optimized output.
# Use -DCMAKE_BUILD_TYPE=Debug when configuring to get debug symbols.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug/Release)" FORCE)
    message(STATUS "No build type specified — defaulting to Release")
endif()
