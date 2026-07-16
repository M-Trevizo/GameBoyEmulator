include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest
        GIT_TAG v2.5.3
)
FetchContent_MakeAvailable(doctest)
list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)
include("${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake")

add_executable(unit_tests tests.cpp ../src/Processor.cpp ../src/Cartridge.cpp)

target_link_libraries(unit_tests PRIVATE doctest::doctest)

include(CTest)
include(doctest)
doctest_discover_tests(unit_tests)

