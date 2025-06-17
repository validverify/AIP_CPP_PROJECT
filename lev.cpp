#include "headers.h"

ImageData::ImageData() : data(nullptr), width(0), height(0), channels(0)
{
}

std::string read_file_to_string(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + file_path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    return content;
}

void qim_embed(const std::string &original, const std::string &stego, const std::string &msg_file,
               const std::string &q_str)
{
    ImageData img;
    img.data = stbi_load(original.c_str(), &img.width, &img.height, &img.channels, 0);
    if (!img.data)
    {
        throw std::runtime_error("Failed to load image");
    }

    std::string msg = read_file_to_string(msg_file);
    std::string message_binary;
    for (char c : msg)
    {
        message_binary += std::bitset<8>(c).to_string();
    }
    message_binary += "00000000";

    size_t index = 0;
    const size_t msg_length = message_binary.length();
    const int total_pixels = img.width * img.height * img.channels;
    int q = std::stoi(q_str);

    for (int i = 0; i < total_pixels; ++i)
    {
        if (index >= msg_length)
            break;

        int v1 = static_cast<int>(img.data[i]);
        int v2 = q * (v1 / q) + (q / 2) * (message_binary[index] - '0');
        img.data[i] = static_cast<unsigned char>(v2);
        ++index;
    }

    stbi_write_png(stego.c_str(), img.width, img.height, img.channels, img.data, img.width * img.channels);
}

void qim_extract(const std::string &stego, const std::string &q_str, const std::string &output_file)
{
    ImageData img;
    img.data = stbi_load(stego.c_str(), &img.width, &img.height, &img.channels, 0);
    if (!img.data)
    {
        throw std::runtime_error("Failed to load image");
    }

    std::string message_binary = "";
    std::string extracted_message = "";
    const int total_pixels = img.width * img.height * img.channels;
    int q = std::stoi(q_str);

    for (int i = 0; i < total_pixels; ++i)
    {
        int v1 = static_cast<int>(img.data[i]);
        int v2 = q * (v1 / q);
        int v3 = q * (v1 / q) + (q / 2);

        if (abs(v1 - v2) < abs(v1 - v3))
        {
            message_binary += '0';
        }
        else
        {
            message_binary += '1';
        }

        if (message_binary.length() >= 8)
        {
            std::string byte = message_binary.substr(0, 8);
            message_binary = message_binary.substr(8);

            if (byte == "00000000")
            {
                break;
            }

            extracted_message += static_cast<char>(std::stoi(byte, nullptr, 2));
        }
    }

    std::ofstream out(output_file, std::ios::binary);
    out.write(extracted_message.c_str(), extracted_message.size());
}

void lsb_embed(const std::string &original, const std::string &stego, const std::string &msg_file)
{
    ImageData img;
    img.data = stbi_load(original.c_str(), &img.width, &img.height, &img.channels, 0);
    if (!img.data)
    {
        throw std::runtime_error("Failed to load image");
    }

    std::string msg = read_file_to_string(msg_file);
    std::string message_binary;
    for (char c : msg)
    {
        message_binary += std::bitset<8>(c).to_string();
    }
    message_binary += "00000000";

    size_t index = 0;
    const size_t msg_length = message_binary.length();
    const int total_pixels = img.width * img.height * img.channels;

    for (int i = 0; i < total_pixels; ++i)
    {
        if (index >= msg_length)
            break;

        int v1 = static_cast<int>(img.data[i]);
        if (v1 % 2 == 0)
        {
            v1 += (message_binary[index] - '0');
        }
        else
        {
            if (message_binary[index] == '0')
            {
                v1 -= 1;
            }
        }

        img.data[i] = static_cast<unsigned char>(v1);
        ++index;
    }

    stbi_write_png(stego.c_str(), img.width, img.height, img.channels, img.data, img.width * img.channels);
}

void lsb_extract(const std::string &stego, const std::string &output_file)
{
    ImageData img;
    img.data = stbi_load(stego.c_str(), &img.width, &img.height, &img.channels, 0);
    if (!img.data)
    {
        throw std::runtime_error("Failed to load image");
    }
    std::string message_binary = "";
    std::string extracted_message = "";
    const int total_pixels = img.width * img.height * img.channels;

    for (int i = 0; i < total_pixels; ++i)
    {
        int v1 = static_cast<int>(img.data[i]);
        if (v1 % 2 == 0)
        {
            message_binary += '0';
        }
        else
        {
            message_binary += '1';
        }

        if (message_binary.length() >= 8)
        {
            std::string byte = message_binary.substr(0, 8);
            message_binary = message_binary.substr(8);

            if (byte == "00000000")
            {
                break;
            }

            extracted_message += static_cast<char>(std::stoi(byte, nullptr, 2));
        }
    }

    std::ofstream out(output_file, std::ios::binary);
    out.write(extracted_message.c_str(), extracted_message.size());
}

void cd_embed(const std::string &original, const std::string &stego, const std::string &msg_file)
{
    ImageData img;
    img.data = stbi_load(original.c_str(), &img.width, &img.height, &img.channels, 0);
    if (!img.data)
    {
        throw std::runtime_error("Failed to load image");
    }
    std::string msg = read_file_to_string(msg_file);
    std::string message_binary;
    for (char c : msg)
    {
        message_binary += std::bitset<8>(c).to_string();
    }
    message_binary += "00000000";

    size_t index = 0;
    const size_t msg_length = message_binary.length();
    const int total_pixels = img.width * img.height;

    for (int i = 0; i < total_pixels; ++i)
    {
        if (index >= msg_length)
            break;

        int r = img.data[i * img.channels + 0];
        int g = img.data[i * img.channels + 1];
        int b = img.data[i * img.channels + 2];
        int diff_rg = r - g;
        int diff_gb = g - b;

        if (abs(diff_rg) < abs(diff_gb))
        {
            if (message_binary[index] == '1')
            {
                if (b % 2 == 0)
                {
                    b += 1;
                }
            }
            else
            {
                if (b % 2 != 0)
                {
                    b -= 1;
                }
            }
        }
        else
        {
            if (message_binary[index] == '1')
            {
                if (r % 2 == 0)
                {
                    r += 1;
                }
            }
            else
            {
                if (r % 2 != 0)
                {
                    r -= 1;
                }
            }
        }

        img.data[i * img.channels + 0] = static_cast<unsigned char>(r);
        img.data[i * img.channels + 2] = static_cast<unsigned char>(b);
        ++index;
    }

    stbi_write_png(stego.c_str(), img.width, img.height, img.channels, img.data, img.width * img.channels);
}

void cd_extract(const std::string &stego, const std::string &output_file)
{
    ImageData img;
    img.data = stbi_load(stego.c_str(), &img.width, &img.height, &img.channels, 0);
    if (!img.data)
    {
        throw std::runtime_error("Failed to load image");
    }
    std::string message_binary = "";
    std::string extracted_message = "";
    const int total_pixels = img.width * img.height;

    for (int i = 0; i < total_pixels; ++i)
    {
        int r = img.data[i * img.channels + 0];
        int g = img.data[i * img.channels + 1];
        int b = img.data[i * img.channels + 2];

        int diff_rg = r - g;
        int diff_gb = g - b;

        if (abs(diff_rg) < abs(diff_gb))
        {
            if (b % 2 == 0)
            {
                message_binary += '0';
            }
            else
            {
                message_binary += '1';
            }
        }
        else
        {
            if (r % 2 == 0)
            {
                message_binary += '0';
            }
            else
            {
                message_binary += '1';
            }
        }

        if (message_binary.length() >= 8)
        {
            std::string byte = message_binary.substr(0, 8);
            message_binary = message_binary.substr(8);

            if (byte == "00000000")
            {
                break;
            }
            extracted_message += static_cast<char>(std::stoi(byte, nullptr, 2));
        }
    }

    std::ofstream out(output_file, std::ios::binary);
    out.write(extracted_message.c_str(), extracted_message.size());
}