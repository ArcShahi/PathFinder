cmake_minimum_required(VERSION 4.2)
project(PathFinder 
    VERSION 2.0
    DESCRIPTION "PathFinder Application"
    LANGUAGES CXX
)

add_executable(pathfinder)

target_sources(pathfinder
        PRIVATE 
        src/main.cpp
        src/pathfinder.cpp
        src/visualizer.cpp 

        PRIVATE 
          FILE_SET HEADERS 
          BASE_DIRS src 
             FILES
               src/cell.hpp
               src/pathfinder.hpp
               src/visualizer.hpp 
)
if(WIN32)
    target_sources(pathfinder PRIVATE icon.rc)
endif()
find_package(raylib CONFIG REQUIRED)
add_subdirectory(vendor/ImGui)
add_subdirectory(vendor/rlImGui)


target_compile_features(pathfinder PRIVATE cxx_std_23)
# Enable Multiprocessor compilation for MSVC only and AVX2 , FMA for all
target_compile_options(pathfinder PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)


# Link libraries
target_link_libraries(pathfinder PRIVATE raylib imgui rlimgui)


