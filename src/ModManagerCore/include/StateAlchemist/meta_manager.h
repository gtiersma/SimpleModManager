#pragma once

#include <switch.h>

#include <string>
#include <vector>

namespace MetaManager {
  
  /**
   * Formats a u64 title ID into a hexidecimal string
   */
  std::string getHexTitleId(const u64& titleId);

  /**
   * Reverse of getHexTitleId
   */
  u64 getNumericTitleId(const std::string& titleId);

  /**
   * Checks if a string is capable of being an actual title ID
   */
  bool isTitleId(const std::string& titleId);

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

  /**
   * Limit string size of the options used by a SelectItem.
   * Needed for the UI. It hangs if strings get to be too excessively long.
   */
  std::vector<std::string> limitSelectLabels(std::vector<std::string>& rawNames);

  void tryResult(Result result);
}