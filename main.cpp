#include "headers.h"
/**
 * \file main.cpp
 * \brief Главный файл программы
 */

/**
 * \brief Обрабатывает аргументы командной строки и вызывает соответствующие функции 
 * для выполнения операций стеганографии с использованием различных методов
 * \param argc Количество аргументов командной строки
 * \param argv Массив аргументов командной строки
 */
int main(int argc, char *argv[])
{
    if ((strcmp(argv[1], "lsb") == 0) && (strcmp(argv[2], "e") == 0))
        lsb_embed(argv[4], argv[5], argv[3]);
    else if ((strcmp(argv[1], "lsb") == 0) && (strcmp(argv[2], "x") == 0))
        lsb_extract(argv[3], argv[4]);
    else if ((strcmp(argv[1], "qim") == 0) && (strcmp(argv[2], "e") == 0))
        qim_embed(argv[4], argv[5], argv[3], argv[6]);
    else if ((strcmp(argv[1], "qim") == 0) && (strcmp(argv[2], "x") == 0))
        qim_extract(argv[3], argv[5], argv[4]);
    else if ((strcmp(argv[1], "cd") == 0) && (strcmp(argv[2], "e") == 0))
        cd_embed(argv[4], argv[5], argv[3]);
    else if ((strcmp(argv[1], "cd") == 0) && (strcmp(argv[2], "x") == 0))
        cd_extract(argv[3], argv[4]);
    else if ((strcmp(argv[1], "cs") == 0) && (strcmp(argv[2], "e") == 0)) {
        ChannelSwapping cs;
        cs.encode(argv[4], argv[5], argv[3]);
    }
    else if ((strcmp(argv[1], "cs") == 0) && (strcmp(argv[2], "x") == 0)) {
        ChannelSwapping cs;
        std::string result = cs.decode(argv[3], std::stoll(argv[4]));
        std::cout << result << std::endl;
    }
    else if ((strcmp(argv[1], "mbc") == 0) && (strcmp(argv[2], "e") == 0)) {
        MidBitChange mbc;
        mbc.encode(argv[4], argv[3], argv[5]);
    }
    else if ((strcmp(argv[1], "mbc") == 0) && (strcmp(argv[2], "x") == 0)) {
        MidBitChange mbc;
        std::string result = mbc.decode(argv[3], std::stoull(argv[4]));
        std::cout << result << std::endl;
    }
    else if ((strcmp(argv[1], "eof") == 0) && (strcmp(argv[2], "e") == 0)) {
        EOFHiding eof;
        eof.encode(argv[4], argv[3], argv[5]);
    }
    else if ((strcmp(argv[1], "eof") == 0) && (strcmp(argv[2], "x") == 0)) {
        EOFHiding eof;
        std::string result = eof.decode(argv[3], std::stoll(argv[4]));
        std::cout << result << std::endl;
    }
    else
        std::cerr << "Error: incorrect arguments" << std::endl;
    return 0;
}