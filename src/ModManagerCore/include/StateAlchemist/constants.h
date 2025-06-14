#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

#include <switch.h>
#include <string>

// Small to minimize memory usage
const s64 FILE_LIST_BUFFER_SIZE = 30;

// Substring to delimit the rating from the mod name in the folder name:
const std::string RATING_DELIMITER = "~~";

// Character at start of a folder name of a source to indicate that it's locked:
const char LOCKED_CHAR = '~';

const std::string TXT_EXT = ".txt";
const std::string ALCHEMIST_PATH = "/mod_alchemy/";
const std::string ATMOSPHERE_PATH = "/atmosphere/contents/";

#endif