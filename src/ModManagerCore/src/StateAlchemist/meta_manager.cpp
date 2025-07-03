#include "StateAlchemist/meta_manager.h"
#include "StateAlchemist/constants.h"
#include <string.h>
#include <AlchemistLogger.h>

/**
 * Formats a u64 title ID into a hexidecimal string
 */
std::string MetaManager::getHexTitleId(const u64& titleId) {
  u64 idCopy = titleId;
  std::string strId;

  // Copilot gave me this; converts to hex
  do {
    strId.insert(strId.begin(), "0123456789abcdef"[idCopy % 16]);
    idCopy >>= 4;
  } while (idCopy != 0);

  // Uppercase letter characters
  for (char& c : strId) {
    c = std::toupper(c);
  }

  // Pad 0s to the left to make it the proper length
  do {
    strId.insert(0, "0");
  } while (strId.size() < 16);

  return strId;
}

/**
 * Reverse of getHexTitleId
 */
u64 MetaManager::getNumericTitleId(const std::string& titleId) {
  std::string idCopy = titleId;

  u64 numericId = 0;
  for (char c : idCopy) {
    numericId <<= 4;
    if (c >= 'A' && c <= 'F') {
      numericId |= (c - 'A' + 10);
    } else if (c >= 'a' && c <= 'f') {
      numericId |= (c - 'a' + 10);
    } else {
      numericId |= (c - '0');
    }
  }

  return numericId;
}

bool MetaManager::isTitleId(const std::string& titleId) {

  // If length is not 16, it's not a title ID
  if (titleId.size() != 16) {
    return false;
  }

  // If there are any non-hex characters, it's not a title ID
  if (titleId.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos) {
    return false;
  }

  return true;
}

/**
 * Lists the names of the games given the vector of title IDs
 * 
 * @return map of title IDs to names
 */
std::map<std::string, std::string> MetaManager::listTitles(const std::vector<u64>& titleIds) {
  alchemyLogger.log("MetaManager::listTitles();");
  std::map<std::string, std::string> titles;

  size_t totalBufferSize = TITLE_BUFFER_SIZE * titleIds.size();
  void* buffer = new (std::align_val_t(0x1000)) u8[totalBufferSize]();

  AsyncValue asyncValue;
  tryResult(
    nsListApplicationTitle(
      &asyncValue,
      NsApplicationControlSource_Storage,
      titleIds.data(),
      titleIds.size(),
      buffer,
      totalBufferSize
    )
  );

  // Fail soft - instead of throwing error, just skip continuing to gather data if something ever fails:
  tryResult(asyncValueWait(&asyncValue, 1'000'000'000));

  s32 offset;
  size_t dataSize;
  tryResult(asyncValueGet(&asyncValue, &offset, sizeof(offset)));
  tryResult(asyncValueGetSize(&asyncValue, &dataSize));

  // If something failed, just use the title IDs as the names
  // We don't want to crash the entire app just because we can't get the names
  if (false) {
    for (size_t i = 0; i < titleIds.size(); i++) {
      std::string hexTitleId = getHexTitleId(titleIds[i]);
      titles[hexTitleId] = hexTitleId;
    }
  } else {
    // Gather names from the buffer
    NacpLanguageEntry* nacpTitles = (NacpLanguageEntry*)((u8*)buffer + offset);
    for (size_t i = 0; i < dataSize / sizeof(NacpLanguageEntry); i++) {
      titles[getHexTitleId(titleIds[i])] = std::string(nacpTitles[i].name);
    }
  }

  delete[] buffer;
  alchemyLogger.log("MetaManager::listTitles: done");

  return titles;
}

/**
 * Parses the name of an entity from a folder name
 */
std::string MetaManager::parseName(const std::string& folderName) {
  std::string name = folderName;

  // Remove the rating substring from the folder name if there is one:
  u8 rating = parseRating(folderName);
  if (rating != 100) {
    name = folderName.substr(0, folderName.length() - RATING_DELIMITER.length() - 2);
  }

  // Remove the locked character from the folder name if there is one:
  if (parseLockedStatus(name)) {
    name = name.substr(1);
  }

  // Now we have the actual name:
  return name;
}

/**
 * Parses a rating of a mod from a folder name
 */
u8 MetaManager::parseRating(const std::string& folderName) {
  if (folderName.length() > RATING_DELIMITER.length() + 2) {

    // Rating should be the last 2 characters:
    std::string possibleRating = folderName.substr(folderName.length() - 2);

    // The rating also gets delimited, so check that there is a delimitor before the digits as well
    std::string possibleDelim = folderName.substr(
      folderName.length() - RATING_DELIMITER.length() - 2,
      RATING_DELIMITER.length()
    );

    // If the last characters check out as a delimitor with two digits, it's safe to assume those digits are the rating:
    if (std::isdigit(possibleRating[0]) && std::isdigit(possibleRating[1]) && possibleDelim == RATING_DELIMITER) {
      return std::stoi(possibleRating);
    }
  }

  // If no rating is detected in the folder name, use the default rating:
  return 100;
}

/**
 * Parses whether a source is locked from randomization or not from the source's folder name
 */
bool MetaManager::parseLockedStatus(const std::string& folderName) {
  return folderName[0] == LOCKED_CHAR;
}

/**
 * Builds a folder name from a mod name and rating
 */
std::string MetaManager::buildFolderName(const std::string& modName, const u8& rating, bool locked) {
  std::string folderName = modName;

  if (rating != 100) {
    std::string ratingStr = std::to_string(rating);

    if (rating < 10) {
      ratingStr.insert(0, "0");
    }

    folderName += RATING_DELIMITER + ratingStr;
  }

  if (locked) {
    folderName.insert(0, 1, LOCKED_CHAR);
  }

  return folderName;
}

/**
 * Checks if the entity name belongs to a folder name
 */
bool MetaManager::namesMatch(char* folderName, const std::string& entityName) {
  std::string folderNameStr(folderName);
  return parseName(folderNameStr) == entityName;
}

/**
 * Throws an error if the Result fails
 */
void MetaManager::tryResult(Result result) {
  if (R_FAILED(result)) {
    fatalThrow(result);
  }
}
