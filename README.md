# Path Finder

A visualization of **Dijkstra's Algorithm** and **A* (A-Star) Algorithm** for pathfinding.

## How to Build

You'll need **CMake** installed on your system.

```sh
# Clone the repository
git clone https://github.com/ArcShahi/PathFinder.git
cd PathFinder

# Create a build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project (default build tools will be used)
cmake --build .

# Find the executable in the Debug or Release directory
./PathFinder.exe
```

## ⚠️ Raylib Dependency

This `CMakeLists.txt` is configured to find **raylib** using **vcpkg**. If you did **not** use vcpkg, update your `CMakeLists.txt` as follows:

```cmake
find_package(raylib 3.0 REQUIRED) # Requires at least version 3.0
```
else CMake will install **Raylib** again.


## TODO
- Optimize performance.
- Implement a **slower visualization mode** to better observe the pathfinding process.
- Improve the **color gradient** for visited paths.
- Make the window more **flexible and resizable**.

---
📌 **Created by ArcShahi**

