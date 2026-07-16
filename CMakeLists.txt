cmake_minimum_required(VERSION 4.1)
project(GameBoyEmulator VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

enable_testing()

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(tests EXCLUDE_FROM_ALL)

add_executable(gameboy WIN32 src/Main.cpp)
target_sources(gameboy PRIVATE
        include/Platform.hpp
        src/Platform.cpp
        include/Processor.hpp
        src/Processor.cpp
        include/Cartridge.hpp
        src/Cartridge.cpp
)

target_link_libraries(gameboy PRIVATE SDL3::SDL3)