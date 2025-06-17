#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "headers.h"
#include <doctest/doctest.h>

namespace fs = std::filesystem;

void create_test_file(const std::string &path, const std::string &content)
{
    std::ofstream file(path, std::ios::binary);
    file << content;
}

void create_test_image(const std::string &path, int width, int height, int channels)
{
    std::vector<unsigned char> pixels(width * height * channels, 128);
    stbi_write_png(path.c_str(), width, height, channels, pixels.data(), width * channels);
}

TEST_CASE("Testing read_file_to_string")
{
    SUBCASE("Reading existing file")
    {
        const std::string test_file = "test_file.txt";
        const std::string content = "Hello World!";
        create_test_file(test_file, content);

        CHECK(read_file_to_string(test_file) == content);
        fs::remove(test_file);
    }

    SUBCASE("Reading non-existent file")
    {
        CHECK_THROWS_AS(read_file_to_string("non_existent_file.txt"), std::runtime_error);
    }
}

TEST_CASE("Testing QIM steganography")
{
    const std::string original_img = "test_qim.png";
    const std::string stego_img = "stego_qim.png";
    const std::string msg_file = "test_qim_msg.txt";
    const std::string output_file = "output_qim_msg.txt";
    const std::string q_value = "4";

    create_test_file(msg_file, "Message");
    create_test_image(original_img, 32, 32, 3);

    SUBCASE("QIM embedding and extraction")
    {
        REQUIRE_NOTHROW(qim_embed(original_img, stego_img, msg_file, q_value));
        REQUIRE_NOTHROW(qim_extract(stego_img, q_value, output_file));

        std::string extracted = read_file_to_string(output_file);
        CHECK(extracted == "Message");
    }

    SUBCASE("QIM embedding with invalid image")
    {
        CHECK_THROWS_AS(qim_embed("non_existent.png", stego_img, msg_file, q_value), std::runtime_error);
    }

    SUBCASE("QIM extraction with invalid image")
    {
        CHECK_THROWS_AS(qim_extract("non_existent.png", q_value, output_file), std::runtime_error);
    }

    fs::remove(original_img);
    fs::remove(stego_img);
    fs::remove(msg_file);
    fs::remove(output_file);
}

TEST_CASE("Testing LSB steganography")
{
    const std::string original_img = "test_lsb.png";
    const std::string stego_img = "stego_lsb.png";
    const std::string msg_file = "test_lsb_msg.txt";
    const std::string output_file = "output_lsb_msg.txt";

    create_test_file(msg_file, "Message");
    create_test_image(original_img, 32, 32, 3);

    SUBCASE("LSB embedding and extraction")
    {
        REQUIRE_NOTHROW(lsb_embed(original_img, stego_img, msg_file));
        REQUIRE_NOTHROW(lsb_extract(stego_img, output_file));

        std::string extracted = read_file_to_string(output_file);
        CHECK(extracted == "Message");
    }

    SUBCASE("LSB embedding with invalid image")
    {
        CHECK_THROWS_AS(lsb_embed("non_existent.png", stego_img, msg_file), std::runtime_error);
    }

    SUBCASE("LSB extraction with invalid image")
    {
        CHECK_THROWS_AS(lsb_extract("non_existent.png", output_file), std::runtime_error);
    }

    fs::remove(original_img);
    fs::remove(stego_img);
    fs::remove(msg_file);
    fs::remove(output_file);
}

TEST_CASE("Testing CD steganography")
{
    const std::string original_img = "test_cd.png";
    const std::string stego_img = "stego_cd.png";
    const std::string msg_file = "test_cd_msg.txt";
    const std::string output_file = "output_cd_msg.txt";

    create_test_file(msg_file, "Message");

    std::vector<unsigned char> pixels(32 * 32 * 3);

    stbi_write_png(original_img.c_str(), 32, 32, 3, pixels.data(), 32 * 3);

    SUBCASE("CD embedding and extraction")
    {
        REQUIRE_NOTHROW(cd_embed(original_img, stego_img, msg_file));
        REQUIRE_NOTHROW(cd_extract(stego_img, output_file));

        std::string extracted = read_file_to_string(output_file);
        CHECK(extracted == "Message");
    }

    SUBCASE("CD embedding with invalid image")
    {
        CHECK_THROWS_AS(cd_embed("non_existent.png", stego_img, msg_file), std::runtime_error);
    }

    SUBCASE("CD extraction with invalid image")
    {
        CHECK_THROWS_AS(cd_extract("non_existent.png", output_file), std::runtime_error);
    }

    fs::remove(original_img);
    fs::remove(stego_img);
    fs::remove(msg_file);
    fs::remove(output_file);
}