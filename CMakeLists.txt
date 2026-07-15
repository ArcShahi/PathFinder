cmake_minimum_required(VERSION 4.2)
project(PathFinder 
    VERSION 3.0
    DESCRIPTION "PathFinder Application"
    LANGUAGES CXX
)

add_executable(pathfinder)

target_sources(pathfinder
        PRIVATE 
        src/App.cpp
        src/State.cpp
        src/Menu.cpp
        src/Solver.cpp
        src/Versus.cpp
        src/Grid.cpp
        src/Astar.cpp
        src/Dijkstra.cpp
        src/UI.cpp
        
        

        PRIVATE 
          FILE_SET HEADERS 
          BASE_DIRS include 
             FILES
               include/State.hpp
               include/StateMachine.hpp
               include/Menu.hpp
               include/Solver.hpp
               include/Versus.hpp
               include/Grid.hpp
               include/IPathfinder.hpp
               include/Astar.hpp
               include/Dijkstra.hpp
               include/UI.hpp
)
if(WIN32)
    target_sources(pathfinder PRIVATE icon.rc)
endif()
find_package(raylib CONFIG REQUIRED)
add_subdirectory(vendor/ImGui)
add_subdirectory(vendor/rlImGui)


target_compile_features(pathfinder PRIVATE cxx_std_23)
# Enable Multiprocessor compilation for MSVC 
target_compile_options(pathfinder PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)

# Link libraries
target_link_libraries(pathfinder PRIVATE raylib imgui rlimgui)


