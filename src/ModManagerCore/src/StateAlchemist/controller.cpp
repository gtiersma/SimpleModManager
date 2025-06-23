#include "StateAlchemist/controller.h"

#include "StateAlchemist/constants.h"
#include "StateAlchemist/fs_manager.h"
#include "StateAlchemist/meta_manager.h"
#include <Logger.h>

Controller controller;


void Controller::setTitleId(const u64& titleId) {
  this->titleId = titleId;

  // Create the Atmosphere title ID folder for the current game
  FsManager::createFolderIfNeeded(this->getAtmospherePath());
}

/**
 * Formats u64 title ID into a hexidecimal string
 */
std::string Controller::getHexTitleId() {
  return MetaManager::getHexTitleId(this->titleId);
}

/**
 * Checks if the currenty-running game has a folder set up for Mod Alchemist
 */
bool Controller::doesGameHaveFolder() {
  return FsManager::doesFolderExist(this->getGamePath());
}

// NOTE: vectors returned from functions in controller
// are implied to be sorted alphabetically unless stated otherwise

/**
 * Load all groups from the game folder
 * 
 * @param sort Whether to sort the list of names alphabetically or not
 *             Can take considerable performance when in nested loops, so sometimes it's good to skip if not needed
 */
std::vector<std::string> Controller::loadGroups(bool sort) {
  return FsManager::listNames(this->getGamePath(), sort);
}

/**
 * Load all source options within the specified group
 * 
 * @param sort Whether to sort the list of names alphabetically or not
 *             Can take considerable performance when in nested loops, so sometimes it's good to skip if not needed
 * 
 * @requirement: group must be set
 */
std::vector<std::string> Controller::loadSources(bool sort) {
  return FsManager::listNames(this->getGroupPath(), sort);
}

/*
 * Gets an unsorted vector of only the sources that are unlocked
 * 
 * @requirement: group must be set
 */
std::vector<std::string> Controller::loadUnlockedSources() {
  std::vector<std::string> sources;

  FsDir dir = FsManager::openFolder(this->getGroupPath(), FsDirOpenMode_ReadDirs);

  FsDirectoryEntry entry;
  s64 readCount = 0;
  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, 1, &entry)) && readCount) {
    if (entry.type == FsDirEntryType_Dir && !MetaManager::parseLockedStatus(entry.name)) {
      sources.push_back(MetaManager::parseName(entry.name));
    }
  }

  fsDirClose(&dir);

  return sources;
}

/*
 * Checks if the source is locked from randomization
 *
 * @requirement: group must be set
 */
bool Controller::isSourceLocked(const std::string& source) {
  bool isLocked;

  FsDir dir = FsManager::openFolder(this->getGroupPath(), FsDirOpenMode_ReadDirs);

  FsDirectoryEntry entry;
  s64 readCount = 0;
  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, 1, &entry))) {
    if (entry.type == FsDirEntryType_Dir && source == MetaManager::parseName(entry.name)) {
      isLocked = MetaManager::parseLockedStatus(entry.name);
      break;
    }
  }

  fsDirClose(&dir);

  return isLocked;
}

/*
 * Load all source options within the specified group along with their lock status
 * 
 * @requirement: group must be set
 */
std::map<std::string, bool> Controller::loadSourceLocks() {
  std::map<std::string, bool> locks;

  FsDir dir = FsManager::openFolder(this->getGroupPath(), FsDirOpenMode_ReadDirs);

  FsDirectoryEntry entry;
  s64 readCount = 0;
  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, 1, &entry)) && readCount) {
    if (entry.type == FsDirEntryType_Dir) {
      std::string source = MetaManager::parseName(entry.name);
      locks[source] = MetaManager::parseLockedStatus(entry.name);
    }
  }

  fsDirClose(&dir);

  return locks;
}

/*
 * Disable randomization for the specified source
 * 
 * @requirement: group must be set
 * @requirement: source must not already be locked
 */
void Controller::lockSource(const std::string& source) {
  u8 rating = this->loadDefaultRating(source);

  std::string currentPath = this->getGroupPath() + "/" + MetaManager::buildFolderName(source, rating, false);
  std::string newPath = this->getGroupPath() + "/" + MetaManager::buildFolderName(source, rating, true);

  FsManager::tryResult(
    fsFsRenameDirectory(
      &FsManager::sdSystem,
      FsManager::toPathBuffer(currentPath).get(),
      FsManager::toPathBuffer(newPath).get()
    )
  );
}

/*
 * Enable randomization for the specified source
 * 
 * @requirement: group must be set
 * @requirement: source must be currently locked
 */
void Controller::unlockSource(const std::string& source) {
  u8 rating = this->loadDefaultRating(source);

  std::string currentPath = this->getGroupPath() + "/" + MetaManager::buildFolderName(source, rating, true);
  std::string newPath = this->getGroupPath() + "/" + MetaManager::buildFolderName(source, rating, false);

  FsManager::tryResult(
    fsFsRenameDirectory(
      &FsManager::sdSystem,
      FsManager::toPathBuffer(currentPath).get(),
      FsManager::toPathBuffer(newPath).get()
    )
  );
}

/**
 * Load all mod options that could be activated for the moddable source in the group
 * 
 * @param sort Whether to sort the list of names alphabetically or not
 *             Can take considerable performance when in nested loops, so sometimes it's good to skip if not needed
 * 
 * @requirement: group and source must be set
 */
std::vector<std::string> Controller::loadMods(bool sort) {
  return FsManager::listNames(this->getSourcePath(), sort);
}


/*
 * Loads map of mods names to each rating
 * 
 * @requirement: group and source must be set
 */
std::map<std::string, u8> Controller::loadRatings() {
  std::map<std::string, u8> ratings;

  FsDir dir = FsManager::openFolder(this->getSourcePath(), FsDirOpenMode_ReadDirs);

  FsDirectoryEntry entry;
  s64 readCount = 0;
  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, 1, &entry)) && readCount) {
    if (entry.type == FsDirEntryType_Dir) {
      std::string mod = MetaManager::parseName(entry.name);
      ratings[mod] = MetaManager::parseRating(entry.name);
    }
  }

  fsDirClose(&dir);

  return ratings;
}

/*
 * Loads the rating for the source (for using no mod)
 * 
 * @requirement: group must be set
 */
u8 Controller::loadDefaultRating(const std::string& source) {
  u8 rating;

  FsDir dir = FsManager::openFolder(this->getGroupPath(), FsDirOpenMode_ReadDirs);

  FsDirectoryEntry entry;
  s64 readCount = 0;
  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, 1, &entry))) {
    if (entry.type == FsDirEntryType_Dir && source == MetaManager::parseName(entry.name)) {
      rating = MetaManager::parseRating(entry.name);
      break;
    }
  }

  fsDirClose(&dir);

  return rating;
}

/*
 * Saves the ratings for each mod
 * 
 * @requirement: group and source must be set
 */
void Controller::saveRatings(const std::map<std::string, u8>& ratings) {
  for (const auto& [mod, rating]: ratings) {
    std::string currentPath = this->getModPath(mod);
    std::string newPath = this->getSourcePath() + "/" + MetaManager::buildFolderName(mod, rating, false);

    FsManager::tryResult(
      fsFsRenameDirectory(
        &FsManager::sdSystem,
        FsManager::toPathBuffer(currentPath).get(),
        FsManager::toPathBuffer(newPath).get()
      )
    );
  }
}

/*
 * Saves the rating for using no mod for the current source
 */
void Controller::saveDefaultRating(const u8& rating) {
  bool isLocked = this->isSourceLocked(this->source);
  std::string newPath = this->getGroupPath() + "/" + MetaManager::buildFolderName(this->source, rating, isLocked);

  FsManager::tryResult(
    fsFsRenameDirectory(
      &FsManager::sdSystem,
      FsManager::toPathBuffer(this->getSourcePath()).get(),
      FsManager::toPathBuffer(newPath).get()
    )
  );
}

/**
 * Gets the mod currently activated for the source
 *
 * Returns an empty string if no mod is active and vanilla files are being used
 * 
 * @requirement: group and must be set
 */
std::string Controller::getActiveMod(const std::string& source) {

  // Open to the correct source directory
  std::string groupPath = this->getGroupPath();
  FsDir sourceDir = FsManager::openFolder(
    groupPath + "/" + FsManager::getFolderName(groupPath, source),
    FsDirOpenMode_ReadFiles
  );

  FsDirectoryEntry entry;
  s64 readCount = 0;
  std::string activeMod = "";
  std::string name;

  // Find the .txt file in the directory. The name would be the active mod:
  while (R_SUCCEEDED(fsDirRead(&sourceDir, &readCount, 1, &entry)) && readCount) {
    if (entry.type == FsDirEntryType_File) {
      name = entry.name;
      if (name.find(TXT_EXT) != std::string::npos) {
        activeMod = name.substr(0, name.size() - TXT_EXT.size());
        break;
      }
    }
  }

  fsDirClose(&sourceDir);

  return activeMod;
}

/**
 * Activates the specified mod, moving all its files into the atmosphere folder for the game
 * 
 * Make sure to deactivate any existing active mod for this source if there is one
 * 
 * Mod won't be activated if EVERY file belonging to it has a conflict with a file already in the atmosphere folder
 * 
 * @requirement:
 *  - group and source must be set
 *  - "mod" parameter must not currently be active
 *  - the title ID folder for the current game must already exist in Atmosphere's "content" folder
 */
void Controller::activateMod(const std::string& mod) {

  // Path to the "mod" folder in alchemy's directory:
  std::string modPath = this->getModPath(mod);

  LogInfo << "Installing files in: " << modPath << std::endl;

  // The txt file for the active mod:
  FsFile movedFilesFile = FsManager::initFile(this->getMovedFilesListFilePath(mod));

  FsDir dir = FsManager::openFolder(modPath, FsDirOpenMode_ReadDirs | FsDirOpenMode_ReadFiles);

  // Iterartor for current entry in the current directory:
  short i = 0;

  // Used for "storing" where the iteration left off at when traversing deeper into the hierarchy:
  std::vector<u64> iStorage;

  // The path we are currently at relative to the mod path.
  // Empty string is mod path itself:
  std::string currentBasePath = "";

  // Position in the txt file where we should write the next file path:
  s64 txtOffset = 0;

  // The index of the current entry we're iterating over in the current directory:
  short entryIndex = 0;

  // The current number of files read at a time
  // It reads 1 at a time, so it will always be either 1 or 0 (0 if all have been read)
  s64 readCount = 0;

  FsDirectoryEntry entry;

  while (R_SUCCEEDED(fsDirRead(&dir, &readCount, 1, &entry))) {

    // Continue iterating the index until it catches up with the iteration we should be on (if needed):
    entryIndex++;
    if (entryIndex > i) {
      i++;

      if (readCount > 0) {
        std::string nextPath = currentBasePath + "/" + entry.name;

        // If the next entry is a file, we will move it and record it as moved as long as there isn't a conflict.
        //
        // File size has to be compared for rare cases where folder is incorrectly categorized as a file.
        // In these cases, the entry loaded is corrupt, so we have to skip it and not load the mod files within it.
        if (entry.type == FsDirEntryType_File && entry.file_size > 0) {

          // If a file already exists in the location we'll move it to, there's a conflict:
          bool fileConflict = FsManager::doesFileExist(this->getAtmospherePath() + nextPath);
          if (!fileConflict) {
            // Record the file we're moving, and move it:
            FsManager::write(movedFilesFile, nextPath + "\n", txtOffset);
            FsManager::moveFile(modPath + nextPath, this->getAtmospherePath() + nextPath);
          }
        // If the next entry is a folder, we will traverse within it:
        } else if (entry.type == FsDirEntryType_Dir) {
          FsManager::createFolderIfNeeded(this->getAtmospherePath() + nextPath);

          // Add the current count to the storage:
          iStorage.push_back(i);

          currentBasePath = nextPath;
          FsManager::changeFolder(dir, modPath + nextPath, FsDirOpenMode_ReadDirs | FsDirOpenMode_ReadFiles);

          // Reset the index & iterator because we're starting in a new folder:
          entryIndex = 0;
          i = 0;
        }
      } else {
        // If there's nothing left in our count storage, we've navigated everything, so we're done:
        if (iStorage.size() == 0) { break; }

        // Otherwise, let's get back the count data of where we left off in the parent:
        i = iStorage.back();
        iStorage.pop_back();

        // Remove the string portion after the last '/' to get the parent's path:
        std::size_t lastSlashIndex = currentBasePath.rfind('/');
        currentBasePath = currentBasePath.substr(0, lastSlashIndex);
        FsManager::changeFolder(dir, modPath + currentBasePath, FsDirOpenMode_ReadDirs | FsDirOpenMode_ReadFiles);

        // Reset the entry index because it will start at the beginning again:
        entryIndex = 0;
      }
    }

  }

  fsFileClose(&movedFilesFile);
  fsDirClose(&dir);
}

/**
 * Deactivates the currently active mod, restoring the moddable source to its vanilla state
 * 
 * @requirement: group and source must be set
 */
void Controller::deactivateMod() {
  std::string activeMod(this->getActiveMod(this->source));

  // If no active mod:
  if (activeMod.empty()) { return; }

  this->returnFiles(activeMod);
}

void Controller::deactivateAll() {
  std::vector<std::string> groups = this->loadGroups(false);

  for (const std::string& group : groups) {
    this->group = group;
    std::vector<std::string> sources = this->loadSources(false);

    for (const std::string& source : sources) {
      this->source = source;
      std::string activeMod(this->getActiveMod(source));

      if (!activeMod.empty()) {
        this->returnFiles(activeMod);
      }
    }
  }

  this->group = "";
  this->source = "";
}

/**
 * Randomly activates/deactivates all mods based upon their ratings
 */
void Controller::randomize() {
  
  // Seed the random number generator with the current time
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  std::vector<std::string> groups = this->loadGroups(false);

  for (const std::string& group : groups) {
    this->group = group;
    std::vector<std::string> sources = this->loadUnlockedSources();

    for (const std::string& source : sources) {
      this->source = source;
      this->pickMod();
    }
  }

  this->group = "";
  this->source = "";
}

/**
 * Randomly activates a mod from the current group and source
 * 
 * @requirement: group and source must be set
 */
void Controller::pickMod() {
  std::map<std::string, u8> ratings = this->loadRatings();
  u8 defaultRating = this->loadDefaultRating(this->source);

  // Sum all ratings to pick one at random:
  u16 ratingTotal = defaultRating;
  for (const auto& [mod, rating]: ratings) {
    ratingTotal += rating;
  }

  // Just treat it as locked if all ratings are 0 for some reason:
  if (ratingTotal == 0) { return; }

  // Get the random number 
  u16 randomChoice = (std::rand() % ratingTotal) + 1;

  // If it's within the default option's range, deactivate it:
  if (randomChoice <= defaultRating) {
    this->deactivateMod();
  } else {
    // Otherwise, keep subtracting the ratings until we reach the one to activate:
    randomChoice -= defaultRating;

    for (const auto& [mod, rating]: ratings) {
      if (randomChoice < rating) {
        std::string activeMod = this->getActiveMod(this->source);

        // No need to do anything if the picked mod is also the currently-active one:
        if (activeMod == mod) { return; }

        // If there's an active mod, deactivate it:
        if (!activeMod.empty()) {
          this->returnFiles(activeMod);
        }

        this->activateMod(mod);
        return;
      }

      randomChoice -= rating;
    }
  }
}

Controller::Controller() {
  pmdmntInitialize();
  pminfoInitialize();

  FsManager::tryResult(fsOpenSdCardFileSystem(&FsManager::sdSystem));
}

/**
 * Unmount SD card when destroyed 
 */
Controller::~Controller() {
  fsFsClose(&FsManager::sdSystem);
  pminfoExit();
  pmdmntExit();
}

/**
 * Returns all files belonging to a mod from the atmosphere active mods folder to their original location
 * 
 * Essentially the same as deactivating the mod, except this can't be used with the default mod option.
 */
void Controller::returnFiles(const std::string& mod) {

  std::unique_ptr<char[]> movedFilesListPath = FsManager::toPathBuffer(this->getMovedFilesListFilePath(mod));
  std::string modPath = this->getModPath(mod);

  // Try to open the active mod's txt file to get the list of files that were moved to atmosphere's folder:
  FsFile movedFilesList;
  FsManager::tryResult(
    fsFsOpenFile(&FsManager::sdSystem, movedFilesListPath.get(), FsOpenMode_Read, &movedFilesList)
  );

  s64 fileSize;
  FsManager::tryResult(
    fsFileGetSize(&movedFilesList, &fileSize)
  );

  // Initialize buffer and path builder:
  s64 offset = 0;
  char* buffer = new char[FILE_LIST_BUFFER_SIZE];
  std::string pathBuilder = "";

  // As long as there is still data in the file:
  while (offset < fileSize) {

    // Read some of the text into our buffer:
    FsManager::tryResult(
      fsFileRead(&movedFilesList, offset, buffer, FILE_LIST_BUFFER_SIZE, FsReadOption_None, nullptr)
    );

    // Append it to the string we're using to build the next path:
    pathBuilder += std::string_view(buffer, FILE_LIST_BUFFER_SIZE);

    // If the path builder got a new line character from the buffer, we have a full path:
    std::size_t newLinePos = pathBuilder.find('\n');
    if (newLinePos != std::string::npos) {
      // Trim the new line and any characters that were gathered after it to get the cleaned atmosphere file path:
      std::string basePath = pathBuilder.substr(0, newLinePos);

      // Move any characters gathered after the new line to the pathBuilder string for the next path:
      pathBuilder = pathBuilder.substr(newLinePos + 1);

      // Move the file back to the mod's folder:
      FsManager::moveFile(
        this->getAtmospherePath() + basePath,
        modPath + basePath
      );

      // Not sure why, but the file needs to be re-opened after each time a file moved:
      fsFileClose(&movedFilesList);
      FsManager::tryResult(
        fsFsOpenFile(&FsManager::sdSystem, movedFilesListPath.get(), FsOpenMode_Read, &movedFilesList)
      );
    }

    offset += FILE_LIST_BUFFER_SIZE;
  }

  delete[] buffer;

  fsFileClose(&movedFilesList);

  // Once all the files have been returned, delete the txt list:
  FsManager::tryResult(
    fsFsDeleteFile(&FsManager::sdSystem, movedFilesListPath.get())
  );
}

/*
 * Gets Mod Alchemist's game directory:
 */
std::string Controller::getGamePath() {
  return ALCHEMIST_PATH + "/" + MetaManager::getHexTitleId(this->titleId);
}

/*
 * Gets the file path for the specified group
 * 
 * @requirement: group must be set
 */
std::string Controller::getGroupPath() {
  return this->getGamePath() + "/" + this->group;
}

/*
 * Gets the file path for the specified source within the group
 * 
 * @requirement: group and source must be set
 */
std::string Controller::getSourcePath() {
  std::string groupPath = this->getGroupPath();
  return groupPath + "/" + FsManager::getFolderName(groupPath, this->source);
}

/*
 * Get the file path for the specified mod within the moddable source
 * 
 * @requirement: group and source must be set
 */
std::string Controller::getModPath(const std::string& mod) {
  std::string sourcePath = this->getSourcePath();
  return sourcePath + "/" + FsManager::getFolderName(sourcePath, mod);
}

/**
 * Gets the game's path that's stored within Atmosphere's directory
 */
std::string Controller::getAtmospherePath() {
  return ATMOSPHERE_PATH + MetaManager::getHexTitleId(this->titleId);
}

/**
 * Gets the file path for the list of moved files for the specified mod
 * 
 * The file should only exist if the mod is currently active
 * 
 * @requirement: group and source must be set
 */
std::string Controller::getMovedFilesListFilePath(const std::string& mod) {
  return this->getSourcePath() + "/" + mod + TXT_EXT;
}
