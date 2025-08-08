//
// Created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_FRAMEMODBROWSER_H
#define SIMPLEMODMANAGER_FRAMEMODBROWSER_H

#include "GuiModManager.h"

#include "borealis.hpp"


class FrameModBrowser : public brls::AppletFrame {

public:
  explicit FrameModBrowser(GuiModManager* guiModManagerPtr_);
  
  [[nodiscard]] const GameBrowser& getGameBrowser() const{ return _guiModManagerPtr_->getGameBrowser(); }
  GameBrowser& getGameBrowser(){ return _guiModManagerPtr_->getGameBrowser(); }

private:
  GuiModManager* _guiModManagerPtr_{};
};


#endif //SIMPLEMODMANAGER_FRAMEMODBROWSER_H
