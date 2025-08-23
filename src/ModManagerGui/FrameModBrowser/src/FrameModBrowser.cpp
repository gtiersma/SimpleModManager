//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "FrameModBrowser.h"

#include <TabModOptions.h>
#include <GroupBrowser.h>

#include <StateAlchemist/controller.h>


void FrameModBrowser::initialize() {
  Game game = gameBrowser.getGame(controller.titleId).value();

  std::string gamePath = controller.getGamePath();

  brls::AppletFrame* appletFrame = (brls::AppletFrame*)this->getContentView();

  // Construct header
  // Header must be cleared and rebuilt because there's no setImageFromMem() for the icon in its API
  brls::Box* header = appletFrame->getHeader();
  header->clearViews(true);
  
  brls::Image* icon = new brls::Image();
  if (game.icon.size() > 0) {
    icon->setImageFromMem(game.icon.data(), 0x20000);
  } else {
    icon->setImageFromFile("romfs:/images/icon_corner.png");
  }

  header->addView(icon);

  brls::Label* title = new brls::Label();
  title->setText(game.name);
  header->addView(title);

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
