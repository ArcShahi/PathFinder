cmake_minimum_required(VERSION 3.3)
project(PathFinder 
    VERSION 1.0
    DESCRIPTION "PathFinder Application"
    LANGUAGES CXX
)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Raylib (Vcpkg)
find_package(raylib CONFIG QUIET)

# If Raylib is not found, use FetchContent to download and build it
if(NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
        
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Add the resource file for Windows icons
if(WIN32)
    set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")
endif()

# Multi processor compilation (Windows MSVC only)
if(MSVC)
    add_compile_options(/MP)
endif()

# Explicitly list source files
# Keep adding it .cpp file else (won't be able to find libraries)
set(SRC_FILES
    src/main.cpp
    src/pathfinder.cpp
    src/visualizer.cpp
   
)

# Add the executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${APP_ICON})

# Link Raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)