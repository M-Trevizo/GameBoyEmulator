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

TEST_CASE("Testing Load Instructions") {
    

    SUBCASE("Testing 16-Bit Loads") {
        processor.PC = 0x0104;
        processor.AF.high = 0x12;
        REQUIRE(processor.PC == 0x0104);

        CHECK(processor.LD_16BIT(processor.BC.word) == 3);
        CHECK(processor.BC.word == 0xCEED);
        CHECK(processor.BC.high == 0xCE);
        CHECK(processor.BC.low == 0xED);

        CHECK(processor.LD_16BIT_A(processor.BC.word) == 2);
        CHECK(processor.memory[processor.BC.word] == 0x12);
    }

    SUBCASE("Testing 8-Bit Loads") {
        
        processor.PC = 0x0104;
        CHECK(processor.LD_8BIT_H(processor.BC.high) == 2);
        CHECK(processor.BC.high == 0xCE);

        processor.PC = 0x0104;
        CHECK(processor.LD_8BIT_L(processor.BC.low) == 2);
        CHECK(processor.BC.low == 0xCE);

        processor.BC.word = 0x0012;
        CHECK(processor.LD_A_16BIT(processor.BC.word) == 2);
        CHECK(processor.memory[processor.BC.word] == 0xFF);
        
        processor.DE.word = 0x0064;
        processor.LD_A_16BIT(processor.DE.word);
        CHECK(processor.memory[processor.DE.word] == 0xE0);
    }

}

TEST_CASE("Testing Arithmetic Instructions") {
    processor.BC.word = 0x11AA;

    SUBCASE("Testing 16-bit Incrememnt and Decrement") {
        CHECK(processor.INC_16BIT(processor.BC.word) == 2);
        CHECK(processor.BC.word == 0x11AA + 1);
        CHECK(processor.DEC_16BIT(processor.BC.word) == 2);
        CHECK(processor.BC.word == 0x11AA);

    }

    processor.BC.word = 0x0001;
    processor.HL.word = 0x0002;
    processor.AF.low |= N_FLAG;
    REQUIRE((processor.AF.low & N_FLAG) == N_FLAG);
    SUBCASE("Testing 16-Bit Add") {
        CHECK(processor.ADD_HL_R16(processor.BC.word) == 2);
        CHECK(processor.HL.word == 0x0003);
        
        SUBCASE("Testing that N-Flag gets unset") {
            CHECK((processor.AF.low & N_FLAG) == 0);
        }

        processor.BC.word = 0x0FFF;
        processor.HL.word = 0x0001;
        SUBCASE("Testing H-Flag") {
            processor.ADD_HL_R16(processor.BC.word);
            uint8_t flag = processor.AF.low & H_FLAG;
            CHECK(flag == H_FLAG);
        }

        processor.BC.word = 0xFFFF;
        processor.HL.word = 0x0001;
        SUBCASE("Testing C-Flag") {
            processor.ADD_HL_R16(processor.BC.word);
            uint8_t flag = processor.AF.low & C_FLAG;
            CHECK(flag == C_FLAG);
        }
    }
}