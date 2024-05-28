cmake_minimum_required(VERSION 3.12)

project(
    luminar
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL)# Update the path accordingly

# Add the SDL2 framework as a dependency
#add_executable(luminar bin/main.cpp lib/Luminar.cpp lib/Camera.cpp lib/Matrix.cpp lib/Vec3.cpp lib/Model.cpp lib/Controller.cpp lib/Vec2.cpp lib/Quat.cpp)
add_subdirectory(bin)
#add_subdirectory(Models)
add_subdirectory(lib/Views)
add_subdirectory(lib/Controllers)

# Link against the SDL2 framework
#target_link_libraries(luminar SDL2)
