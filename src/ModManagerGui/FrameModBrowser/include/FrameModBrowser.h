//
// Created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_FRAMEMODBROWSER_H
#define SIMPLEMODMANAGER_FRAMEMODBROWSER_H

#include "GuiModManager.h"

#include "autofocusless_tab_frame.hpp"


class FrameModBrowser : public brls::AutofocuslessTabFrame {

public:
  explicit FrameModBrowser(GuiModManager* guiModManagerPtr_);
  bool onCancel() override;
  
  [[nodiscard]] const GameBrowser& getGameBrowser() const{ return _guiModManagerPtr_->getGameBrowser(); }
  GameBrowser& getGameBrowser(){ return _guiModManagerPtr_->getGameBrowser(); }

private:
  GuiModManager* _guiModManagerPtr_{};
};


#endif //SIMPLEMODMANAGER_FRAMEMODBROWSER_H
