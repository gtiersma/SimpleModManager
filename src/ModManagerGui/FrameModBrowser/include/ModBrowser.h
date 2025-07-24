//
// Created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_MODBROWSER_H
#define SIMPLEMODMANAGER_MODBROWSER_H


#include <GuiModManager.h>
#include "ModManager.h"

#include <borealis.hpp>

#include "string"


class FrameModBrowser;

class ModBrowser : public brls::List {

public:
  explicit ModBrowser(FrameModBrowser* owner_);

  void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;

  ModManager& getModManager();

  void loadMods(std::string group);

private:
  FrameModBrowser* _owner_{nullptr};

  // Label used for the setting to turn a mod off
  const std::string _DEFAULT_LABEL_{"UNMODIFIED"}; 
};


#endif //SIMPLEMODMANAGER_MODBROWSER_H
