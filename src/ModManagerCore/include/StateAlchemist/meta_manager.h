#pragma once

#include <switch.h>

#include <string>

namespace MetaManager {
  
  /**
   * Formats a u64 title ID into a hexidecimal string
   */
  std::string getHexTitleId(const u64& titleId);

  /**
   * Parses the name of an entity from a folder name
   */
  std::string parseName(const std::string& folderName);

  /**
   * Parses a rating of a mod from a folder name
   */
  u8 parseRating(const std::string& folderName);

  /**
   * Parses whether a source is locked from randomization or not from the source's folder name
   */
  bool parseLockedStatus(const std::string& folderName);

  /**
   * Builds a folder name from a mod name and rating
   */
  std::string buildFolderName(const std::string& modName, const u8& rating, bool locked);

  /**
   * Checks if the entity name belongs to a folder name
   */
  bool namesMatch(char* folderName, const std::string& entityName);
}