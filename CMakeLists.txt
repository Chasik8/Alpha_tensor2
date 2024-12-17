cmake_minimum_required(VERSION 3.26)
project(Alpha_tensor2)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Укажите путь к TBB вручную
set(TBB_ROOT "${CMAKE_SOURCE_DIR}/oneapi-tbb-2022.0.0")
set(TBB_INCLUDE_DIR "${TBB_ROOT}/include")
set(TBB_LIBRARY_DIR "${TBB_ROOT}/lib/intel64/vc14")

# Проверка существования директорий TBB
if (NOT EXISTS ${TBB_INCLUDE_DIR})
    message(FATAL_ERROR "TBB include directory does not exist: ${TBB_INCLUDE_DIR}")
endif()
if (NOT EXISTS ${TBB_LIBRARY_DIR})
    message(FATAL_ERROR "TBB library directory does not exist: ${TBB_LIBRARY_DIR}")
endif()

# Path to Torch
set(Torch_DIR "D:/Project/C++/CLion/Alpha_tensor2/libtorch/share/cmake/Torch")
set(CMAKE_PREFIX_PATH "D:/Project/C++/CLion/Alpha_tensor2/libtorch")

# Find the Torch package
find_package(Torch REQUIRED)

# Add the executable target
add_executable(${PROJECT_NAME} main.cpp main.h vmul3.cpp vmul3.h derevo.cpp derevo.h tqdm.hpp read.cpp read.h)

# Подключение TBB
target_include_directories(${PROJECT_NAME} PRIVATE ${TBB_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${TBB_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE tbb)

# Подключение Torch
target_include_directories(${PROJECT_NAME} PUBLIC "${TORCH_INCLUDE_DIRS}")
target_link_libraries(${PROJECT_NAME} PRIVATE "${TORCH_LIBRARIES}")
target_compile_options(${PROJECT_NAME} PUBLIC ${TORCH_CXX_FLAGS})

# Debug output
message(STATUS "TBB_INCLUDE_DIR: ${TBB_INCLUDE_DIR}")
message(STATUS "TBB_LIBRARY_DIR: ${TBB_LIBRARY_DIR}")
message(STATUS "TORCH_INCLUDE_DIRS: ${TORCH_INCLUDE_DIRS}")
message(STATUS "TORCH_LIBRARIES: ${TORCH_LIBRARIES}")

# Copy Torch DLLs after build (MSVC only)
if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()
