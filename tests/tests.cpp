#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include "../include/Processor.hpp"

string path = "E:/GB-Roms/Tetris.gb";
Cartridge cartridge(path);
Processor processor(cartridge);

TEST_CASE("Testing Cartridge") {
    std::cout << (int)cartridge.cartROM[0] << std::endl;
    CHECK(cartridge.cartROM[0x0101] == 0xC3);
}

TEST_CASE("Testing Processor") {
    CHECK(processor.memory[0x0101] == 0xC3);
}

TEST_CASE("Testing Fetch and that PC increments after each Fetch call") {
    CHECK(processor.PC == 0x0100);
    CHECK(processor.fetch() == 0);
    CHECK(processor.PC == 0x0101);
    CHECK(processor.fetch() == 0xC3);
}

TEST_CASE("Testing Load instructions") {
    processor.PC = 0x0104;
    REQUIRE(processor.PC == 0x0104);

    SUBCASE("Testing 16-Bit Loads") {
        CHECK(processor.LD_16BIT(processor.BC.word) == 3);
        CHECK(processor.BC.word == 0xCEED);
        CHECK(processor.BC.high == 0xCE);
        CHECK(processor.BC.low == 0xED);
    }

}