//
// Created by Adrien BLANCHET on 20/06/2020.
//

#include "TabAbout.h"
#include <Toolbox.h>

#include "Logger.h"

#include <borealis.hpp>
#include <AlchemistLogger.h>



LoggerInit([]{
  Logger::setUserHeaderStr("[TabAbout]");
});

TabAbout::TabAbout() {
  alchemyLogger.log("TabAbout::TabAbout();");
  LogWarning << "Building about tab..." << std::endl;

  // Subtitle
  auto* shortDescription = new brls::Label(
    brls::LabelStyle::REGULAR,
    "Simple Mod Alchemist is an Nintendo Switch homebrew app for managing mods on the SD card.\n"\
    "It originated by merging the code of two homebrew apps together: SimpleModManager and State Alchemist.\n",
    true
  );
  shortDescription->setHorizontalAlign(NVG_ALIGN_CENTER);
  this->addView(shortDescription);

  auto* table = new brls::BoxLayout(brls::BoxLayoutOrientation::HORIZONTAL);
  table->setSpacing(22);
  table->setHeight(260);

  auto* leftBox = new brls::BoxLayout(brls::BoxLayoutOrientation::VERTICAL);
  leftBox->setSpacing(22);
  leftBox->setWidth(500);
  leftBox->setParent(table);

  leftBox->addView(new brls::Header("Version " + Toolbox::getAppVersion() + " - What's new ?"));
  auto *changelog = new brls::Label(
    brls::LabelStyle::DESCRIPTION,
    " - Mods are now grouped\n"\
    " - Major performance improvements with large files\n"\
    " - Improved how mods are organized in folders\n"\
    " - The best part: General stability improvements\n",
    true
  );
  changelog->setHorizontalAlign(NVG_ALIGN_LEFT);
  leftBox->addView(changelog);

  leftBox->addView(new brls::Header("Copyright"));
  auto *copyright = new brls::Label(
    brls::LabelStyle::DESCRIPTION,
    "Simple Mod Alchemist is licensed under GPL-v3.0\n" \
        "\u00A9 SimpleModManager 2019 - 2023 Nadrino"\
        "\u00A9 Simple Mod Alchemist 2025 gtiersma",
    true
  );
  copyright->setHorizontalAlign(NVG_ALIGN_CENTER);
  leftBox->addView(copyright);

  auto* rightBox = new brls::BoxLayout(brls::BoxLayoutOrientation::VERTICAL);
  rightBox->setSpacing(22);
  rightBox->setWidth(200);
  rightBox->setParent(table);

  rightBox->addView(new brls::Label(brls::LabelStyle::DESCRIPTION, " "));

  auto* portrait = new brls::Image("romfs:/images/portrait.jpg");
  portrait->setScaleType(brls::ImageScaleType::SCALE);
  portrait->setHeight(200);
  portrait->setParent(rightBox);
  rightBox->addView(portrait);
  auto* portraitText = new brls::Label(
    brls::LabelStyle::SMALL,
    "SimpleModManager Original Author: Nadrino",
    true
  );
  portraitText->setHorizontalAlign(NVG_ALIGN_CENTER);
  rightBox->addView(portraitText);

  table->addView(leftBox);
  table->addView(rightBox);

  this->addView(table);

  this->addView(new brls::Header("Credits"));
  auto *links = new brls::Label(
    brls::LabelStyle::SMALL,
    "\uE017  Maintained by gtiersma.\n"\
    "\uE017  Built upon SimpleModManager, developed by Nadrino.\n"\
    "\uE017  Powered by Borealis, provided by the RetroNX team.\n"\
    "\uE017  Special thanks to RetroNX, devkitPro,"\
    " the ethical homebrew development community in general, and Nintendo.\n"\
    "\uE017  This software is NOT licensed by Nintendo.\n",
    true
  );
  this->addView(links);

  LogInfo << "About tab built." << std::endl;
}
