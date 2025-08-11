//
// Created by Adrien BLANCHET on 22/06/2020.
//

#include "FrameRoot.h"

#include <TabGames.h>
#include <TabAbout.h>
#include <TabGeneralSettings.h>


FrameRoot::FrameRoot() {
  this->setTitle("Simple Mod Alchemist (v" + APP_VERSION + ")");
  this->setIcon("romfs:/images/icon_corner.png");

  brls::TabFrame* tabs = new brls::TabFrame();
  tabs->addTab("Game Browser", [this](){ return new TabGames(this); });
  tabs->addSeparator();
  tabs->addTab("Options", [this](){ return new TabGeneralSettings(this); });
  tabs->addTab("About", [](){ return new TabAbout(); });

  this->addView(tabs);
}
