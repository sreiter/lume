#todo: rename to slimesh once it is in its own repository
project (slimesh_core)
cmake_minimum_required(VERSION 3.1)

set (sources
        subset_info_annex.cpp
        file_io.cpp
        grob.cpp
        mesh.cpp
        neighborhoods.cpp
        neighbors.cpp
        normals.cpp
        tests.cpp
        topology.cpp
    )

include_directories (deps/rapidxml-1.13)
include_directories (deps)

file(COPY "test_meshes" DESTINATION "${CMAKE_BINARY_DIR}")

add_library (lume ${sources})

add_executable (lume_tests tests_main.cpp)
target_link_libraries(lume_tests lume)

