//
// Created by Adrien BLANCHET on 22/06/2020.
//

#ifndef SIMPLEMODMANAGER_TABMODOPTIONS_H
#define SIMPLEMODMANAGER_TABMODOPTIONS_H

#include <ModBrowser.h>

#include "ModManager.h"

#include <borealis.hpp>


class FrameModBrowser;

class TabModOptions : public brls::Box {

public:
  explicit TabModOptions(FrameModBrowser* owner_);

  [[nodiscard]] const ModManager& getModManager() const;
  ModManager& getModManager();

  void initialize();

  void buildDisableAllMods();

private:
  FrameModBrowser* _owner_{nullptr};
  brls::DetailCell* _itemDisableAllMods_{nullptr};

};


#endif //SIMPLEMODMANAGER_TABMODOPTIONS_H
