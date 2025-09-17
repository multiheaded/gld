cmake_minimum_required(VERSION 4.0)
project(gridlock_defenders)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(archon
        GIT_REPOSITORY https://github.com/janreitz/archon.git
        GIT_TAG main
        GIT_SHALLOW ON)
FetchContent_MakeAvailable(archon)

FetchContent_Declare(nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
        GIT_SHALLOW ON)
FetchContent_MakeAvailable(nlohmann_json)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(gridlock_defenders src/main.cpp
        src/Game.cpp
        src/integration/SFML/Renderer.cpp
        src/integration/SFML/Window.cpp
        src/Battlefield.cpp
        include/AssetManager.h
        src/AssetManager.cpp
        src/Components.cpp
        src/UI.cpp
        include/UI.h
        include/ConcurrentQueue.h
)

target_include_directories(gridlock_defenders PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gridlock_defenders PRIVATE SFML::Window SFML::Graphics archon Eigen3::Eigen nlohmann_json::nlohmann_json)

#[[
target_compile_options(gridlock_defenders PRIVATE
    -Wall                    # Enable most warning messages
    -Wextra                  # Enable extra warning messages
    -Werror                  # Treat warnings as errors
    -Wpedantic               # Strict ISO C++ compliance
    -Wshadow                 # Warn about variable shadowing
    -Wnon-virtual-dtor       # Warn about non-virtual destructors
    -Wold-style-cast         # Warn about C-style casts
    -Wcast-align             # Warn about pointer casts that increase alignment
    -Wunused                 # Warn about unused variables/functions
    -Woverloaded-virtual     # Warn about overloaded virtual functions
    -Wconversion             # Warn about type conversions
    -Wsign-conversion        # Warn about sign conversions
    -Wmisleading-indentation # Warn about misleading indentation
    -Wnull-dereference       # Warn about null dereferences
    -Wdouble-promotion       # Warn about double promotions
    -Wformat=2               # Extra format string checking
    # GCC-specific warnings
    $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-cond>
    $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-branches>
    $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op>
    $<$<CXX_COMPILER_ID:GNU>:-Wuseless-cast>
)]]
