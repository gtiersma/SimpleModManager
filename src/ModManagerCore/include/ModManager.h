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
struct ModSource {
  ModSource() = default;

  /**
   * @param source_ String label of the name of the moddable thing (source)
   * @param mods_ List of available mods of the source
   * @param activeIndex_ Index of "mods" vector of mod that is currently active (-1 if none active)
   */
  explicit ModSource(std::string source_, std::vector<std::string> mods_, size_t activeIndex_):
    source(std::move(source_)),
    mods(std::move(mods_)),
    activeIndex(activeIndex_) {
      options = mods_;
      options.insert(options.begin(), _DEFAULT_OPTION_);
    }

  std::string source;
  std::vector<std::string> mods;

  // Essentially just the mods list, but as options for the UI.
  // The main difference is it also has the option for using no mod at the beginning as an additional element.
  std::vector<std::string> options;

  size_t activeIndex;

  // Option used for the setting to turn a mod off
  const std::string _DEFAULT_OPTION_{"UNMODIFIED"}; 
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

  // shortcuts
  const ConfigHolder& getConfig() const;
  ConfigHolder& getConfig();

  // selector related
  ModSource loadSource(const std::string& sourceName);

private:
  GameBrowser* _owner_{nullptr};

  // mod management
  int getActiveIndex(const std::string& sourceName, const std::vector<std::string>& mods);
};


#endif //MODAPPLIER_MOD_MANAGER_H
