#ifndef MOD_MIGRATOR_H
#define MOD_MIGRATOR_H

#include <string>

/**
 * Migrates old mods set up for vanilla SimpleModManager to the new format to work with this manager
 */
class ModMigrator {
  public:
    static void begin();

  private:
    static const std::string LEGACY_BASE_PATH;
    static const std::string LEGACY_MOD_ROOT_FOLDER;
    static const std::string MIGRATION_GROUP;
    static const std::string MIGRATION_MOD_NAME;

    static void migrateGame(const std::string& gameFolder);
    static void migrateMod(
      const std::string& groupPath,
      const std::string& modFolder,
      const std::string& titleId,
      const std::string& legacyGamePath
    );
    static void moveFiles(const std::string& oldPath, const std::string& newPath);
};

#endif //MOD_MIGRATOR_H