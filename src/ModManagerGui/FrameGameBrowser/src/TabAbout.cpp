//
// Created by Adrien BLANCHET on 20/06/2020.
//

#include "TabAbout.h"
#include <StateAlchemist/constants.h>
#include <AlchemistLogger.h>

TabAbout::TabAbout() {
  alchemyLogger.log("TabAbout:::TabAbout");
  
  // Subtitle
  brls::Label* shortDescription = new brls::Label();
  shortDescription->setText(
    "Simple Mod Alchemist is an Nintendo Switch homebrew app for managing mods on the SD card.\n"\
    "It originated by merging the code of two homebrew apps together: SimpleModManager and State Alchemist.\n"
  );
  shortDescription->setHorizontalAlign(brls::HorizontalAlign::CENTER);
  this->addView(shortDescription);

  brls::Box* columns = new brls::Box(brls::Axis::ROW);
  columns->setJustifyContent(brls::JustifyContent::SPACE_BETWEEN);
  this->addView(columns);

  brls::Box* leftBox = new brls::Box();
  leftBox->setJustifyContent(brls::JustifyContent::SPACE_BETWEEN);
  leftBox->setWidth(500);
  columns->addView(leftBox);

  brls::Header* versionHeader = new brls::Header();
  versionHeader->setTitle("Version " + APP_VERSION + " - What's new ?");
  leftBox->addView(versionHeader);

  brls::Label* changelog = new brls::Label();
  changelog->setText(
    " - Mods are now grouped\n"\
    " - Major performance improvements with large files\n"\
    " - Improved how mods are organized in folders\n"\
    " - The best part: General stability improvements\n"
  );
  changelog->setHorizontalAlign(brls::HorizontalAlign::LEFT);
  leftBox->addView(changelog);

  brls::Header* copyrightHeader = new brls::Header();
  copyrightHeader->setTitle("Copyright");
  leftBox->addView(copyrightHeader);

  brls::Label* copyright = new brls::Label();
  copyright->setText(
    "Simple Mod Alchemist is licensed under GPL-v3.0\n" \
        "\u00A9 SimpleModManager 2019 - 2023 Nadrino"\
        "\u00A9 Simple Mod Alchemist 2025 gtiersma"
  );
  copyright->setHorizontalAlign(brls::HorizontalAlign::CENTER);
  leftBox->addView(copyright);

  brls::Box* rightBox = new brls::Box();
  rightBox->setJustifyContent(brls::JustifyContent::SPACE_BETWEEN);
  rightBox->setWidth(200);
  columns->addView(rightBox);

  brls::Image* portrait = new brls::Image();
  portrait->setImageFromFile("romfs:/images/portrait.jpg");
  portrait->setScalingType(brls::ImageScalingType::FIT);
  rightBox->addView(portrait);

  brls::Label* portraitLabel = new brls::Label();
  portraitLabel->setText("SimpleModManager Original Author: Nadrino");
  portraitLabel->setFontSize(15.0f);
  portraitLabel->setHorizontalAlign(brls::HorizontalAlign::CENTER);
  rightBox->addView(portraitLabel);

  this->addView(columns);

  brls::Header* creditsHeader = new brls::Header();
  creditsHeader->setTitle("Credits");
  this->addView(creditsHeader);

  brls::Label* credits = new brls::Label();
  credits->setText(
    "\u00A9  Maintained by gtiersma.\n"\
    "\u00A9  Built upon SimpleModManager, developed by Nadrino.\n"\
    "\u00A9  Powered by Borealis, provided by the RetroNX team.\n"\
    "\u00A9  Special thanks to RetroNX, devkitPro,"\
    " the ethical homebrew development community in general, and Nintendo.\n"\
    "\u00A9  This software is NOT licensed by Nintendo.\n"
  );
  credits->setFontSize(15.0f);
  this->addView(credits);
}

brls::View* TabAbout::create() { return new TabAbout(); }
