#include "AlchemistLogger.h"
#include <StateAlchemist/controller.h>

AlchemistLogger alchemyLogger;

AlchemistLogger::AlchemistLogger() {
  controller.init();

  rawPath = "/The Log of the Alchemist.txt";
  path = FsManager::toPathBuffer(rawPath);

  if (FsManager::doesFileExist(rawPath)) {
    fsFsDeleteFile(&FsManager::sdSystem, path.get());
  }

  fsFsCreateFile(&FsManager::sdSystem, path.get(), 0, 0);

  fsFsOpenFile(&FsManager::sdSystem, path.get(), FsOpenMode_Write | FsOpenMode_Append, &file);
}
  
AlchemistLogger::~AlchemistLogger() {
  fsFileClose(&file);
}

void AlchemistLogger::log(const std::string& message) {
  fsFileWrite(&file, offset, message.c_str(), message.size(), FsWriteOption_Flush);
  offset += message.size();
}