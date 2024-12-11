cmake_minimum_required(VERSION 3.26)
project(Alpha_tensor2)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to Torch
set(Torch_DIR "D:/Project/C++/CLion/Alpha_tensor2/libtorch/share/cmake/Torch")
set(CMAKE_PREFIX_PATH "D:/Project/C++/CLion/Alpha_tensor2/libtorch")

#set(CMAKE_EXE_LINKER_FLAGS "-static")
# Find the Torch package
find_package(Torch REQUIRED)

# Optional: Set CUDA compiler paths if using CUDA
if (CMAKE_CUDA_COMPILER)
    set(CMAKE_CUDA_COMPILER "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8/bin/nvcc.exe")
    set(CMAKE_CUDA_HOST_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe")
    enable_language(CUDA)
endif()

# Add the executable target
add_executable(Alpha_tensor2 main.cpp main.h vmul3.cpp vmul3.h derevo.cpp derevo.h tqdm.hpp read.cpp read.h)

# Link Torch libraries
target_link_libraries(Alpha_tensor2 "${TORCH_LIBRARIES}")

# Include directories for Torch (optional for some installations)
target_include_directories(Alpha_tensor2 PUBLIC "${TORCH_INCLUDE_DIRS}")

# Set flags from Torch (important for compilation)
target_compile_options(Alpha_tensor2 PUBLIC ${TORCH_CXX_FLAGS})

# For MSVC, copy Torch DLLs after build
if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET Alpha_tensor2
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS}
            $<TARGET_FILE_DIR:Alpha_tensor2>)
endif()
