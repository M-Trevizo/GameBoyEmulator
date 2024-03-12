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

    SUBCASE("Testing 8-Bit Increments and Decrements") {

        // Testing Increment
        processor.BC.word = 0x1010;
        CHECK(processor.INC_8BIT(processor.BC.low) == 1);
        CHECK(processor.INC_8BIT(processor.BC.high) == 1);
        CHECK(processor.BC.low == 0x11);
        CHECK(processor.BC.high == 0x11);

        // Testing Decrement
        processor.BC.word = 0x1010;
        CHECK(processor.DEC_8BIT(processor.BC.low) == 1);
        CHECK(processor.DEC_8BIT(processor.BC.high) == 1);
        CHECK(processor.BC.low == 0x0F);
        CHECK(processor.BC.high == 0x0F);
        

        SUBCASE("Testing flags for Increment Instruction") {

            SUBCASE("Test Z flag is set") {
                processor.AF.low = 0x00;
                processor.BC.low = 0xFF;
                processor.INC_8BIT(processor.BC.low);
                CHECK(processor.BC.low == 0);
                processor.AF.low &= Z_FLAG;
                CHECK(processor.AF.low == Z_FLAG);

                processor.AF.low = 0x00;
                processor.BC.high = 0xFF;
                processor.INC_8BIT(processor.BC.high);
                CHECK(processor.BC.high == 0);
                processor.AF.low &= Z_FLAG;
                CHECK(processor.AF.low == Z_FLAG);
            }

            SUBCASE("Test H flag is set") {
                processor.AF.low = 0x00;
                processor.BC.low = 0x0F;
                processor.INC_8BIT(processor.BC.low);
                CHECK(processor.BC.low == 0x10);
                processor.AF.low &= H_FLAG;
                CHECK(processor.AF.low == H_FLAG);

                processor.AF.low = 0x00;
                processor.BC.high = 0x0F;
                processor.INC_8BIT(processor.BC.high);
                CHECK(processor.BC.low == 0x10);
                processor.AF.low &= H_FLAG;
                CHECK(processor.AF.low == H_FLAG);
            }

            SUBCASE("Test N flag is unset") {
                processor.AF.low = 0x00;
                processor.BC.low = 0x01;
                processor.INC_8BIT(processor.BC.low);
                processor.AF.low &= N_FLAG;
                CHECK(processor.AF.low == 0x0);

                processor.AF.low = 0x00;
                processor.BC.high = 0x01;
                processor.INC_8BIT(processor.BC.high);
                processor.AF.low &= N_FLAG;
                CHECK(processor.AF.low == 0x0);
            }
        }

        SUBCASE("Testing flags for Decrement Instruction") {

            SUBCASE("Test Z flag is set") {
                processor.AF.low = 0x00;
                processor.BC.low = 0x01;
                processor.DEC_8BIT(processor.BC.low);
                CHECK(processor.BC.low == 0);
                processor.AF.low &= Z_FLAG;
                CHECK(processor.AF.low == Z_FLAG);
            }

            SUBCASE("Test H flag is set") {
                processor.AF.low = 0x00;
                processor.BC.low = 0x10;
                processor.DEC_8BIT(processor.BC.low);
                CHECK(processor.BC.low == 0x0F);
                processor.AF.low &= H_FLAG;
                CHECK(processor.AF.low == H_FLAG);
            }

            SUBCASE("Test N flag is set") {
                processor.AF.low = 0x00;
                processor.BC.low = 0x01;
                processor.DEC_8BIT(processor.BC.low);
                processor.AF.low &= N_FLAG;
                CHECK(processor.AF.low == N_FLAG);
            }
        }
    }

    SUBCASE("Testing Add Instructions") {

        SUBCASE("Testing 16-bit add") {
            processor.HL.word = 0x0;
            processor.BC.word = 0x1010;
            processor.ADD_HL_R16(processor.BC.word);
            CHECK(processor.HL.word == 0x1010);

            SUBCASE("Testing flags") {

                SUBCASE("Test H flag is set") {
                    processor.AF.low = 0x0;
                    processor.HL.word = 0x0F00;
                    processor.BC.word = 0x0100;
                    processor.ADD_HL_R16(processor.BC.word);
                    processor.AF.low &= H_FLAG;
                    CHECK(processor.AF.low == H_FLAG);
                }

                SUBCASE("Test C flag is set") {
                    processor.AF.low = 0x0;
                    processor.HL.word = 0xF000;
                    processor.BC.word = 0x1000;
                    processor.ADD_HL_R16(processor.BC.word);
                    processor.AF.low &= C_FLAG;
                    CHECK(processor.AF.low == C_FLAG);
                }

                SUBCASE("Test N flag is unset") {
                    processor.AF.low = 0x0;
                    processor.HL.word = 0xF000;
                    processor.BC.word = 0x1000;
                    processor.ADD_HL_R16(processor.BC.word);
                    processor.AF.low &= N_FLAG;
                    CHECK(processor.AF.low == 0x0);
                }
            }
        }
    }
}

TEST_CASE("Testing Rotate/Shift Bit Instructions") {
    
    SUBCASE("Testing RLCA") {
        processor.AF.low = 0x0;
        processor.AF.high = 0x88;
        processor.RLCA();
        CHECK(processor.AF.high == 0x11);
        
        SUBCASE("Testing Flags") {
            
            SUBCASE("Testing Z flag is unset") {
                processor.AF.low = 0x0;
                processor.AF.high = 0x88;
                processor.RLCA();
                processor.AF.low &= Z_FLAG;
                CHECK(processor.AF.low == 0x0);
            }

            SUBCASE("Testing N flag is unset") {
                processor.AF.low = 0x0;
                processor.AF.high = 0x88;
                processor.RLCA();
                processor.AF.low &= N_FLAG;
                CHECK(processor.AF.low == 0x0);
            }

            SUBCASE("Testing H flag is unset") {
                processor.AF.low = 0x0;
                processor.AF.high = 0x88;
                processor.RLCA();
                processor.AF.low &= H_FLAG;
                CHECK(processor.AF.low == 0x0);
            }

            SUBCASE("Testing C flag is set") {
                processor.AF.low = 0x0;
                processor.AF.high = 0x88;
                processor.RLCA();
                processor.AF.low &= C_FLAG;
                CHECK(processor.AF.low == C_FLAG);
            }
        }
    }

    SUBCASE("Testing RRCA") {
        processor.AF.low = 0x0;
        processor.AF.high = 0x01;
        processor.RRCA();
        CHECK(processor.AF.high == 0x80);
        
        SUBCASE("Testing Flags") {
            
            SUBCASE("Testing Z flag is unset") {
                processor.AF.low = 0x0;
                processor.AF.high = 0x01;
                processor.RRCA();
                processor.AF.low &= Z_FLAG;
                CHECK(processor.AF.low == 0x0);
            }

            SUBCASE("Testing N flag is unset") {
                processor.AF.low = 0x0;
                processor.AF.high = 0x01;
                processor.RRCA();
                processor.AF.low &= N_FLAG;
                CHECK(processor.AF.low == 0x0);
            }

            SUBCASE("Testing H flag is unset") {
                processor.AF.low = 0x0;
                processor.AF.high = 0x01;
                processor.RRCA();
                processor.AF.low &= H_FLAG;
                CHECK(processor.AF.low == 0x0);
            }

            SUBCASE("Testing C flag is set") {
                processor.AF.low = 0x0;
                processor.AF.high = 0x01;
                processor.RRCA();
                processor.AF.low &= C_FLAG;
                CHECK(processor.AF.low == C_FLAG);
            }
        }
    }
}