#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

#include <string>

const std::string APP_VERSION = "1.0.0";

// Small to minimize memory usage
const int64_t FILE_LIST_BUFFER_SIZE = 30;

// Max number of folders/files to load at a time when processing them
const int MAX_FS_ENTRY_LOAD = 50;

// Substring to delimit the rating from the mod name in the folder name:
const std::string RATING_DELIMITER = "~~";

// Character at start of a folder name of a source to indicate that it's locked:
const char LOCKED_CHAR = '~';

const std::string TXT_EXT = ".txt";
const std::string ALCHEMIST_FOLDER = "mod_alchemy";
const std::string ALCHEMIST_PATH = "/" + ALCHEMIST_FOLDER;
const std::string ATMOSPHERE_PATH = "/atmosphere/contents/";

// UI seems likely to hang if a control's label is much longer than this number
const int MAX_LABEL_SIZE = 25;

#endif