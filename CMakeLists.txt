cmake_minimum_required(VERSION 3.10)
project (HLVulkan)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# ======= Vulkan =======
find_package(Vulkan)

# ======= My code ======= 

# Includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Sources
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCES ${SRC_DIR}/buffer.cpp 
            ${SRC_DIR}/command_pool.cpp 
            ${SRC_DIR}/device.cpp 
            ${SRC_DIR}/fence.cpp
            ${SRC_DIR}/helper.cpp
            ${SRC_DIR}/image.cpp
            ${SRC_DIR}/shader.cpp
)
# Build the executable
set(LIBRARY HLVulkan)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library(${LIBRARY} SHARED ${SOURCES})
target_link_libraries(${LIBRARY} Vulkan::Vulkan)