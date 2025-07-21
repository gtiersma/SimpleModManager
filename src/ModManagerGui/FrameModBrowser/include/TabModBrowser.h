//
// Created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_TABMODBROWSER_H
#define SIMPLEMODMANAGER_TABMODBROWSER_H


#include <GuiModManager.h>
#include "ModManager.h"

#include <borealis.hpp>

#include "string"


class FrameModBrowser;

class TabModBrowser : public brls::List {

public:
  explicit TabModBrowser(FrameModBrowser* owner_, std::string group_);

  void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;

  ModManager& getModManager();

  void reloadActiveMods();

  static bool _shouldReloadActiveMods_;

private:
  FrameModBrowser* _owner_{nullptr};
  std::string _group_;
  std::vector<brls::SelectListItem*> _items_;
  std::vector<ModSource> _mods_;

  // Label used for the setting to turn a mod off
  const std::string _DEFAULT_LABEL_{"UNMODIFIED"}; 
};


#endif //SIMPLEMODMANAGER_TABMODBROWSER_H
