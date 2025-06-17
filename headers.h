/**
 * \file lev.h
 * \brief Заголовочный файл, содержащий объявления функций, используемых для стеганографических операций
 */

#ifndef LEV_H
#include "stb_image.h"
#include "stb_image_write.h"
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>

/**
 * \brief Структура для хранения данных изображения
 *
 * Содержит указатель на данные изображения, а также информацию о его размерах
 * и количестве цветовых каналов.
 */
struct ImageData
{
	unsigned char *data;
	int width;
	int height;
	int channels;

	ImageData();
	~ImageData();
};

/**
 * \brief Читает содержимое файла в строку
 *
 * \param file_path Путь к файлу для чтения
 * \return std::string Содержимое файла в виде строки
 * \throw std::runtime_error Если файл не может быть открыт
 */
std::string read_file_to_string(const std::string &file_path);

/**
 * \brief Встраивает сообщение в изображение методом QIM
 * \param original Путь к исходному изображению
 * \param stego Путь для сохранения стего-изображения
 * \param msg_file Путь к файлу с встраиваемым сообщением
 * \param q_str Параметр квантования
 */
void qim_embed(const std::string &original, const std::string &stego, const std::string &msg_file,
			   const std::string &q_str);

/**
 * \brief Извлекает сообщение из стего-изображения методом QIM
 * \param stego Путь к стего-изображению
 * \param q_str Параметр квантования
 * \param output_file Путь для сохранения извлеченного сообщения
 */
void qim_extract(const std::string &stego, const std::string &q_str, const std::string &output_file);

/**
 * \brief Встраивает сообщение в изображение методом LSB
 * \param original Путь к исходному изображению
 * \param stego Путь для сохранения стего-изображения
 * \param msg_file Путь к файлу с сообщением для встраивания
 */
void lsb_embed(const std::string &original, const std::string &stego, const std::string &msg_file);

/**
 * \brief Извлекает сообщение из стего-изображения методом LSB
 * \param stego Путь к стего-изображению
 * \param output_file Путь для сохранения извлеченного сообщения
 */
void lsb_extract(const std::string &stego, const std::string &output_file);

/**
 * \brief Встраивает сообщение в изображение методом CD
 * \param original Путь к исходному изображению
 * \param stego Путь для сохранения стего-изображения
 * \param msg_file Путь к файлу с сообщением для встраивания
 */
void cd_embed(const std::string &original, const std::string &stego, const std::string &msg_file);

/**
 * \brief Извлекает сообщение из стего-изображения методом CD
 * \param stego Путь к стего-изображению
 * \param output_file Путь для сохранения извлеченного сообщения
 */
void cd_extract(const std::string &stego, const std::string &output_file);


// Marlen part

/**
 * @brief BasicImage is class that realise basic work with image throw stb_image
 * module.
 *
 * @throw CAN_NOT_LOAD_IMAGE_FILE(LOAD_IMAGE_PIXELS:CAN_NOT_LOAD_IMAGE_FILE) -
 * When image loader in stb_image can not open image file;
 * @throw CAN_NOT_SAVE_IMAGE - When function save_result can not save image with
 * redacted data throw stb_image module;
 *
 * @return In constructor function class returns own object with
 * accessible(public) functions: get_image_loader, get_image_params,
 * get_pixels_range, save_result, free_space
 */
class BasicImage
{
public:
	/**
	 * @brief Constructor for class BasicImage, that helps work with images.
	 *
	 * @param img_path Constant that contains path to image file.
	 * @return BasicImage class.
	 */
	BasicImage(const std::string &img_path);

	/**
	 * @brief Just return loader object of image
	 *
	 * @return unsigned char image.
	 */
	unsigned char *get_image_loader();

	/**
	 * @brief Function to get image basic params such a Width, Height, Channels.
	 *
	 * @return Integer vector with 3 params.
	 */
	std::vector<int> get_image_params();

	/**
	 * @brief Just return result of image assigned params.
	 *
	 * @return Unsigned char vector with pixels dataset.
	 */
	std::vector<unsigned char> get_pixels_range();

	/**
	 * @brief Function that saves result of working with image and free space.
	 *
	 * @param output_path Constant value of output image path.
	 */
	void save_result(const std::string &output_path, std::vector<unsigned char> new_pixels);

	/**
	 * @brief Function to free space that contains image data
	 */
	void free_space();

private:
	int w, h, ch;
	unsigned char *loaded_image;
	std::vector<unsigned char> pixels;
};

/**
 * @brief ChannelSwapping is class that realise encode and decode functionality
 * of Channel Swapping encrypt algotithm.
 *
 * @throw TOO_MANY_SENSETIVE_DATA_TO_ENCODE - When encode function got too big
 * sensetive data message even image can hold this;
 * @throw IMAGE_CAN_NOT_BE_OPEN_TO_DECODE - When image loader on stb_image
 * module can not open image file;
 */
class ChannelSwapping
{
public:
	/**
	 * @brief Encode function with Channel Swapping algorithm.
	 *
	 * @param img_path Constant that contains path to input image file.
	 * @param sens_data Sensetive data to encode.
	 * @param output_path Constant that contains path to output image file, save
	 * file.
	 */
	void encode(const std::string &img_path, const std::string &sens_data, const std::string &output_path);

	/**
	 * @brief Decode function with Channel Swapping algorithm.
	 *
	 * @param img_path Constant that contains path to input image file.
	 * @param sens_data_size Lenght of data, that was encoded to image.
	 *
	 * @return String value that is result of decoding proccess.
	 */
	std::string decode(const std::string &img_path, long long int sens_data_size);

	/**
	 * @brief Get size of last encoded data
	 */
	long long int get_last_encoded_size() const;

private:
	long long int last_encoded_size = 0;
};

/**
 * @brief MidBitChange is class that realise encode and decode functionality of
 * Mid Bit Changing encrypt algotithm.
 *
 * @throw MESSAGE_TO_LARGE_FOR_IMAGE - When encode function got too big
 * sensetive data message even image can hold this;
 * @throw FAILED_TO_SAVE_IMAGE - When image loader on stb_image module can not
 * save redacted image in encode function;
 */
class MidBitChange
{
public:
	/**
	 * @brief Encode function with Mid Bit Changing algorithm.
	 *
	 * @param img_path Constant that contains path to input image file.
	 * @param sens_data Sensetive data to encode.
	 * @param output_path Constant that contains path to output image file, save
	 * file.
	 */
	void encode(const std::string &img_path, const std::string &sens_data, const std::string &output_path);

	/**
	 * @brief Decode function with Mid Bit Changing algorithm.
	 *
	 * @param img_path Constant that contains path to input image file.
	 * @param sens_data_size Lenght of data, that was encoded to image.
	 *
	 * @return String value that is result of decoding proccess.
	 */
	std::string decode(const std::string &img_path, const size_t sens_data_size);
};

/**
 * @brief EOFHiding is class that realise encode and decode functionality of End
 * Of File hidding algotithm.
 *
 * @throw FILE_CAN_NOT_BE_OPEN - Universal error that appears when image file
 * can not be open.
 * @throw FILE_IS_EMPTY - universal Error that appears when image file is empty.
 * @throw SENS_DATA_SIZE_IS_INCCORECT - When sensetive data size is negative or
 * equals zero.
 */
class EOFHiding
{
public:
	/**
	 * @brief Hidding(encoding) sensetive data in the end of file (EOF
	 * Steganography).
	 *
	 * @param img_path Path to original(container) image file.
	 * @param sens_data Sensetive data to hide.
	 * @param output_path Output file path and name.
	 */
	void encode(const std::string &img_path, const std::string &sens_data, const std::string &output_path);

	/**
	 * @brief Taking(decoding) sensetive data from th end of file.
	 *
	 * @param img_path Path to container image file.
	 * @param sens_data_size Lenght of hidden sensetive data.
	 *
	 * @return String data that is result of decoding.
	 */
	std::string decode(const std::string &img_path, long long int sens_data_size);
};


#endif