cmake_minimum_required(VERSION 4.0)
project(gridlock_defenders)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(gridlock_defenders src/main.cpp
        src/Game.cpp
        src/integration/SFML/Renderer.cpp
        src/integration/SFML/Window.cpp
)

target_include_directories(gridlock_defenders PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gridlock_defenders PRIVATE SFML::Window SFML::Graphics)

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
)