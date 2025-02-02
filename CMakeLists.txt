cmake_minimum_required(VERSION 3.3)
project(PathFinder CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Raylib (Vcpkg)
find_package(raylib CONFIG QUIET)

# If Raylib is not found, use FetchContent to download and build it
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(raylib)
endif()


# Add the resource file for Windows icons
if (WIN32)
    set(APP_ICON icon.rc)
endif()

# Add the executable and link the icon (if on Windows)
add_executable(PathFinder src/main.cpp ${APP_ICON}) # Add ${APP_ICON} here

# Link Raylib
target_link_libraries(PathFinder PRIVATE raylib)
