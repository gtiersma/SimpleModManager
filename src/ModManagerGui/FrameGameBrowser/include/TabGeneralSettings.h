//
// Created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_TABGENERALSETTINGS_H
#define SIMPLEMODMANAGER_TABGENERALSETTINGS_H

#include "ConfigHandler.h"

#include <borealis.hpp>


class FrameRoot;

class TabGeneralSettings : public brls::Box {

public:
  explicit TabGeneralSettings();
  static brls::View* create();

  void rebuildLayout();

  brls::Box* itemInstallLocationPreset{nullptr};

  [[nodiscard]] const ConfigHolder& getConfig() const;
  ConfigHolder& getConfig();

private:
  brls::Dialog* buildMigrateFinishedDialog();
};


#endif //SIMPLEMODMANAGER_TABGENERALSETTINGS_H
