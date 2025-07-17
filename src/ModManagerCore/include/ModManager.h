//
// Created by Nadrino on 06/09/2019.
//

#ifndef MODAPPLIER_MOD_MANAGER_H
#define MODAPPLIER_MOD_MANAGER_H

#include <ConfigHandler.h>

#include <map>
#include <vector>
#include <string>
#include <utility>


/**
 * Object containing data related to a "source" (something moddable in a game)
 */
struct ModSource{
  ModSource() = default;

  /**
   * @param source_ String label of the name of the moddable thing
   * @param mods_ List of available mods for the moddable thing
   */
  explicit ModSource(std::string source_, std::vector<std::string> mods_): source(std::move(source_)), mods(std::move(mods_)) {}

  std::string source;
  std::vector<std::string> mods;
};


class GameBrowser;

/**
 * Class for interacting with mods
 * 
 * Originally part of vanilla SimpleModManager. Now uses StateAlchemist as a backend.
 * 
 * Set the group in controller.group; all interactions in this class are scoped to that group.
 */
class ModManager {

public:
  explicit ModManager(GameBrowser* owner_);

  /**
   * Gets a list of all ModSources that belong to the current group
   */
  const std::vector<ModSource> &getGroupedModList() const;

  // shortcuts
  const ConfigHolder& getConfig() const;
  ConfigHolder& getConfig();

  // selector related
  void updateModList();

  // mod management
  int getActiveIndex(const ModSource& source_);

private:
  GameBrowser* _owner_{nullptr};

  /**
   * List of mods only belonging to group that is current when constructed.
   * Can always be reconstructed by calling updateModList if the group changes.
   */
  std::vector<ModSource> _groupedModList_{};
};


#endif //MODAPPLIER_MOD_MANAGER_H
