//
// Created by Adrien BLANCHET on 22/06/2020.
//

#include "FrameRoot.h"

#include <TabGames.h>
#include <TabAbout.h>
#include <TabGeneralSettings.h>


#include "Toolbox.h"


FrameRoot::FrameRoot() {
  this->setTitle("Simple Mod Alchemist (v" + Toolbox::getAppVersion() + ")");
  this->setIcon("romfs:/images/icon_corner.png");

  brls::TabFrame* tabs = new brls::TabFrame();
  tabs->addTab("Game Browser", [this](){ return new TabGames(this); });
  tabs->addSeparator();
  tabs->addTab("Options", [this](){ new TabGeneralSettings(this); });
  tabs->addTab("About", [](){ return new TabAbout(); });

  this->addView(tabs);
}
