#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "methods.cpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

const std::string ORIGINAL_IMAGE = "original.png";
const std::string TEST_MESSAGE = "TestSecretMessage123";

TEST_SUITE("BasicImage Tests") {
    TEST_CASE("Load original image") {
        std::cout << "Loading image from: " << fs::absolute(ORIGINAL_IMAGE) << std::endl;
        
        if (!fs::exists(ORIGINAL_IMAGE)) {
            FAIL("Test image 'original.png' not found in directory: ", 
                 fs::current_path().string());
        }

        SUBCASE("Check image loading") {
            BasicImage image(ORIGINAL_IMAGE);
            CHECK(image.get_image_loader() != nullptr);
            
            auto params = image.get_image_params();
            CHECK(params[0] > 0);  // width
            CHECK(params[1] > 0);  // height
            CHECK(params[2] == 3); // channels
        }

        SUBCASE("Save image copy") {
            const std::string output = "test_output.png";
            BasicImage image(ORIGINAL_IMAGE);
            std::vector<unsigned char> pixels = image.get_pixels_range();
            image.save_result(output, pixels);
            CHECK(fs::exists(output));
            if (fs::exists(output)) fs::remove(output);
        }
    }
}

TEST_SUITE("ChannelSwapping Tests") {
    TEST_CASE("Encode and decode") {
        if (!fs::exists(ORIGINAL_IMAGE)) {
            FAIL("Original image not found");
        }
        const std::string encoded_img = "encoded_cs.png";
        
        ChannelSwapping cs;
        
        SUBCASE("Encode message") {
            cs.encode(ORIGINAL_IMAGE, TEST_MESSAGE, encoded_img);
            CHECK(fs::exists(encoded_img));
        }

        SUBCASE("Decode message") {
            if (!fs::exists(encoded_img)) {
                FAIL("Encoded image not found");
            }
            std::string decoded = cs.decode(encoded_img, TEST_MESSAGE.size());
            std::cout << decoded << " " << TEST_MESSAGE<< std::endl;
            CHECK(decoded == TEST_MESSAGE);
            if (fs::exists(encoded_img)) fs::remove(encoded_img);
        }
    }
}

TEST_SUITE("MidBitChange Tests") {
    TEST_CASE("Encode and decode") {
        if (!fs::exists(ORIGINAL_IMAGE)) {
            FAIL("Original image not found");
        }
        const std::string encoded_img = "encoded_mbc.png";
        
        MidBitChange mbc;
        
        SUBCASE("Encode message") {
            mbc.encode(ORIGINAL_IMAGE, TEST_MESSAGE, encoded_img);
            CHECK(fs::exists(encoded_img));
        }

        SUBCASE("Decode message") {
            if (!fs::exists(encoded_img)) {
                FAIL("Encoded image not found");
            }
            std::string decoded = mbc.decode(encoded_img, TEST_MESSAGE.size());
            CHECK(decoded == TEST_MESSAGE);
            if (fs::exists(encoded_img)) fs::remove(encoded_img);
        }
    }
}

TEST_SUITE("EOFHiding Tests") {
    TEST_CASE("Append and extract data") {
        if (!fs::exists(ORIGINAL_IMAGE)) {
            FAIL("Original image not found");
        }
        const std::string encoded_img = "encoded_eof.png";
        
        EOFHiding eof;
        
        SUBCASE("Append data") {
            eof.encode(ORIGINAL_IMAGE, TEST_MESSAGE, encoded_img);
            CHECK(fs::exists(encoded_img));
        }

        SUBCASE("Extract data") {
            if (!fs::exists(encoded_img)) {
                FAIL("Encoded image not found");
            }
            std::string decoded = eof.decode(encoded_img, TEST_MESSAGE.size());
            CHECK(decoded == TEST_MESSAGE);
            if (fs::exists(encoded_img)) fs::remove(encoded_img);
        }
    }
}