//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "FrameModBrowser.h"

#include <TabModOptions.h>
#include <GroupBrowser.h>

#include <StateAlchemist/controller.h>

#include <icon_applet.hpp>


void FrameModBrowser::initialize() {
  Game game = gameBrowser.getGame(controller.titleId).value();

  brls::IconApplet* appletFrame = (brls::IconApplet*)this->getContentView();
  
  if (game.icon.size() > 0) {
    appletFrame->setIconFromMem(game.icon.data(), 0x20000);
  } else {
    appletFrame->setIconFromRes("/img/icon_corner.png");
  }

  appletFrame->setTitle(game.name);

  brls::Label* footerLabel = new brls::Label();
  footerLabel->setText("Simple Mod Alchemist");
  appletFrame->getFooter()->addView(footerLabel);

  appletFrame->getContentView()->registerAction("Back to Game Selection", brls::BUTTON_B, [](brls::View* view) {
    brls::Application::popActivity(brls::TransitionAnimation::SLIDE_RIGHT);

    // clear the group/source shown
    controller.source = "";
    controller.group = "";

    return true;
  });
}
