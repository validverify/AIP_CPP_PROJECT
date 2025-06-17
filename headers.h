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

#endif