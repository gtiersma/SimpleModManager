//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "FrameModBrowser.h"

#include <TabModOptions.h>
#include <GroupBrowser.h>

#include <StateAlchemist/controller.h>

#include <icon_applet.hpp>


using namespace brls::literals;

void FrameModBrowser::initialize() {
  Game game = gameBrowser.getGame(controller.titleId).value();

  brls::IconApplet* appletFrame = (brls::IconApplet*)this->getContentView();

  brls::TabFrame* tabs = (brls::TabFrame*)appletFrame->getContentView();
  brls::Sidebar* sidebar = (brls::Sidebar*)tabs->getChildren().at(0);
  sidebar->setWidthPercentage(25.0f);
  
  if (game.icon.size() > 0) {
    appletFrame->setIconFromMem(game.icon.data(), 0x20000);
  } else {
    appletFrame->setIconFromRes("/img/icon_corner.png");
  }

  appletFrame->setTitle(game.name);

  tabs->registerAction("Back to Game Selection", brls::BUTTON_B, [](brls::View* view) {
    brls::Application::popActivity();

    // clear the group/source shown
    controller.source = "";
    controller.group = "";

    return true;
  });
}
