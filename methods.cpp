#include "headers.h"

BasicImage::BasicImage(const std::string &img_path)
{
    loaded_image = stbi_load(img_path.c_str(), &w, &h, &ch, 3);

    if (!loaded_image)
    {
        std::cerr << "Error: CAN NOT LOAD IMAGE FILE." << img_path << std::endl;
        throw std::runtime_error("LOAD_IMAGE_PIXELS:CAN_NOT_LOAD_IMAGE_FILE.");
    }

    pixels.assign(loaded_image, loaded_image + w * h * 3);
}

unsigned char *BasicImage::get_image_loader()
{
    return loaded_image;
}

std::vector<int> BasicImage::get_image_params()
{
    return {w, h, ch};
}

std::vector<unsigned char> BasicImage::get_pixels_range()
{
    return pixels;
}

void BasicImage::save_result(const std::string &output_path, std::vector<unsigned char> new_pixels)
{
    if (!stbi_write_png(output_path.c_str(), w, h, 3, new_pixels.data(), w * 3))
    {
        std::cerr << "Error: CAN NOT SAVE IMAGE." << std::endl;
    }
    stbi_image_free(loaded_image);
}

void BasicImage::free_space()
{
    stbi_image_free(loaded_image);
}

void ChannelSwapping::encode(const std::string &img_path, const std::string &sens_data, const std::string &output_path)
{
    BasicImage image(img_path);
    int width = image.get_image_params()[0], height = image.get_image_params()[1],
        channels = image.get_image_params()[2];
    std::vector<unsigned char> pixels = image.get_pixels_range();

    const long long int total_bits = sens_data.size() * 8;
    if (total_bits > width * height)
    {
        throw std::runtime_error("Error: TOO_MANY_SENSETIVE_DATA_TO_ENCODE: Message too large for the image");
    }

    long long int bit_pos = 0;
    for (long long int i = 0; i < pixels.size() && bit_pos < total_bits; i += 3)
    {
        char current_char = sens_data[bit_pos / 8];
        bool bit = (current_char >> (7 - (bit_pos % 8))) & 1; // Get bit pos than inverse and shifts to this pos comp with 00000001 

        if (bit)
        {
            // R < G
            if (pixels[i] <= pixels[i + 1])
            {
                std::swap(pixels[i], pixels[i + 1]);
            }
        }
        else
        {
            // R > G
            if (pixels[i] >= pixels[i + 1])
            {
                std::swap(pixels[i], pixels[i + 1]);
            }
        }
        bit_pos++;
    }

    image.save_result(output_path, pixels);
    last_encoded_size = sens_data.size();
}

std::string ChannelSwapping::decode(const std::string &img_path, long long int sens_data_size)
{
    BasicImage image(img_path);
    int width = image.get_image_params()[0], height = image.get_image_params()[1],
        channels = image.get_image_params()[2];
    unsigned char *img = image.get_image_loader();

    std::string bit_string;
    const long long int total_bits = sens_data_size * 8;

    for (long long int i = 0; i < width * height * 3 && bit_string.size() < total_bits; i += 3)
    {
        bit_string += (img[i] > img[i + 1]) ? '1' : '0';
    }

    image.free_space();

    std::string result;
    for (long long int i = 0; i + 8 <= bit_string.size() && result.size() < sens_data_size; i += 8)
    {
        result += static_cast<char>(std::bitset<8>(bit_string.substr(i, 8)).to_ulong());
    }

    return result;
}

long long int ChannelSwapping::get_last_encoded_size() const
{
    return last_encoded_size;
}

void MidBitChange::encode(const std::string &img_path, const std::string &sens_data, const std::string &output_path)
{
    BasicImage image(img_path);
    std::vector<unsigned char> pixels = image.get_pixels_range();
    const size_t total_bits = sens_data.size() * 8;

    if (total_bits > pixels.size())
    {
        std::cerr << "Error: Message too large for the image." << std::endl;
        return;
    }

    size_t bit_pos = 0;
    for (size_t i = 0; i < pixels.size() && bit_pos < total_bits; ++i)
    {
        if (i % 3 == 2)
            continue;

        char current_char = sens_data[bit_pos / 8];
        bool bit = (current_char >> (7 - (bit_pos % 8))) & 1;
        pixels[i] = (pixels[i] & 0xEF) | (bit << 4);
        bit_pos++;
    }

    if (!stbi_write_png(output_path.c_str(), image.get_image_params()[0], image.get_image_params()[1],
                        image.get_image_params()[2], pixels.data(), image.get_image_params()[0] * 3))
    {
        std::cerr << "Error: Failed to save image." << std::endl;
    }
}

std::string MidBitChange::decode(const std::string &img_path, const size_t sens_data_size)
{
    BasicImage image(img_path);
    std::vector<unsigned char> pixels = image.get_pixels_range();
    std::string binary_str;

    for (size_t i = 0; i < pixels.size() && binary_str.size() < sens_data_size * 8; ++i)
    {
        if (i % 3 == 2)
            continue;
        bool bit = (pixels[i] >> 4) & 1;
        binary_str += (bit ? '1' : '0');
    }

    std::string result;
    for (size_t i = 0; i + 8 <= binary_str.size(); i += 8)
    {
        result += static_cast<char>(std::bitset<8>(binary_str.substr(i, 8)).to_ulong());
    }

    return result;
}

void EOFHiding::encode(const std::string &img_path, const std::string &sens_data, const std::string &output_path)
{
    std::ifstream in(img_path, std::ios::binary);
    if (!in.is_open())
    {
        throw std::runtime_error("Error FILE_CAN_NOT_BE_OPEN: Failed to open input file '" + img_path + "'");
    }

    in.seekg(0, std::ios::end);
    if (in.tellg() == 0)
    {
        throw std::runtime_error("Error FILE_IS_EMPTY: Input file '" + img_path + "' is empty");
    }

    in.seekg(0, std::ios::beg);
    std::ofstream out(output_path, std::ios::binary);

    if (!out.is_open())
    {
        throw std::runtime_error("Error FILE_CAN_NOT_BE_OPEN: Failed to open input file '" + output_path + "'");
    }

    out << in.rdbuf();
    out.write(sens_data.c_str(), sens_data.size());
}

std::string EOFHiding::decode(const std::string &img_path, long long int sens_data_size)
{
    if (sens_data_size <= 0)
    {
        throw std::runtime_error("Error SENS_DATA_SIZE_IS_INCCORRECT: Invalid data size (must be positive)");
    }

    std::ifstream in(img_path, std::ios::binary | std::ios::ate);
    if (!in.is_open())
    {
        throw std::runtime_error("Error FILE_CAN_NOT_BE_OPEN: Failed to open input file '" + img_path + "'");
    }

    long long int fileSize = in.tellg();
    in.seekg(fileSize - sens_data_size);

    std::vector<char> hidden_data(sens_data_size);
    in.read(hidden_data.data(), sens_data_size);

    return std::string(hidden_data.begin(), hidden_data.end());
}
