#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

#include <string>

// Small to minimize memory usage
const int64_t FILE_LIST_BUFFER_SIZE = 30;

// Substring to delimit the rating from the mod name in the folder name:
const std::string RATING_DELIMITER = "~~";

// Character at start of a folder name of a source to indicate that it's locked:
const char LOCKED_CHAR = '~';

const std::string TXT_EXT = ".txt";
const std::string ALCHEMIST_FOLDER = "mod_alchemy";
const std::string ALCHEMIST_PATH = "/" + ALCHEMIST_FOLDER;
const std::string ATMOSPHERE_PATH = "/atmosphere/contents/";

// Size in bytes to reserve for storing a singlegame's title
const size_t TITLE_BUFFER_SIZE = 4096;

#endif