//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "FrameModBrowser.h"

#include <TabModPresets.h>
#include <TabModOptions.h>
#include <GroupBrowser.h>

#include "GenericToolbox.Switch.h"
#include <StateAlchemist/controller.h>
#include <AlchemistLogger.h>


FrameModBrowser::FrameModBrowser() {
  alchemyLogger.log("FrameModBrowser::FrameModBrowser...");
  Game game = gameBrowser.getGame(controller.titleId).value();

  std::string gamePath = controller.getGamePath();

  // Construct header
  // Header must be cleared and rebuilt because there's no setImageFromMem() for the icon in its API
  alchemyLogger.log("FrameModBrowser::Clearing header...");
  brls::Box* header = this->getHeader();
  header->clearViews(true);
  
  alchemyLogger.log("FrameModBrowser::Setting header image...");
  brls::Image* icon = new brls::Image();
  if (game.icon.size() > 0) {
    icon->setImageFromMem(game.icon.data(), 0x20000);
  } else {
    icon->setImageFromFile("romfs:/images/icon_corner.png");
  }

  alchemyLogger.log("FrameModBrowser::Adding image...");
  header->addView(icon);

  alchemyLogger.log("FrameModBrowser::Adding title...");
  brls::Label* title = new brls::Label();
  title->setText(game.name);
  header->addView(title);

  alchemyLogger.log("FrameModBrowser::Setting footer...");
  brls::Label* footerLabel = new brls::Label();
  footerLabel->setText("Simple Mod Alchemist");
  this->getFooter()->addView(footerLabel);

  brls::TabFrame* tabs = new brls::TabFrame();
  tabs->addTab("Mod Browser", []() { return new GroupBrowser(); });
  tabs->addSeparator();
  tabs->addTab("Options", []() {
    TabModOptions* tabModOptions = new TabModOptions();
    tabModOptions->initialize();
    return tabModOptions;
  });

  tabs->registerAction("Back to Game Selection", brls::BUTTON_B, [](brls::View* view) {
    brls::Application::popActivity(brls::TransitionAnimation::SLIDE_RIGHT);

    // clear the group/source shown
    controller.source = "";
    controller.group = "";

    return true;
  });

  alchemyLogger.log("FrameModBrowser::Adding tabs...");
  this->addView(tabs);
}
