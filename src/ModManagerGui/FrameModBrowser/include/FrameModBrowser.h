//
// Created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_FRAMEMODBROWSER_H
#define SIMPLEMODMANAGER_FRAMEMODBROWSER_H

#include <TabModPlugins.h>
#include <TabModOptions.h>
#include <TabModPresets.h>

#include "GuiModManager.h"
#include "GameBrowser.h"

#include <borealis.hpp>

#include "TabGroupBrowser.h"


class FrameModBrowser : public brls::TabFrame {

public:
  explicit FrameModBrowser(GuiModManager* guiModManagerPtr_);
  bool onCancel() override;

  uint8_t *getIcon();
  TabModPresets* getTabModPresets(){ return _tabModPresets_; }


  [[nodiscard]] const GuiModManager& getGuiModManager() const{ return *_guiModManagerPtr_; }
  [[nodiscard]] const GameBrowser& getGameBrowser() const{ return _guiModManagerPtr_->getGameBrowser(); }

  GuiModManager& getGuiModManager() { return *_guiModManagerPtr_; }
  GameBrowser& getGameBrowser(){ return _guiModManagerPtr_->getGameBrowser(); }


private:
  GuiModManager* _guiModManagerPtr_{};

  // memory handled by brls
  TabGroupBrowser* _tabGroupBrowser_{nullptr};
  TabModOptions* _tabModOptions_{nullptr};
  TabModPresets* _tabModPresets_{nullptr};
  TabModPlugins* _tabModPlugins_{nullptr};

  uint8_t* _icon_{nullptr};

};


#endif //SIMPLEMODMANAGER_FRAMEMODBROWSER_H
