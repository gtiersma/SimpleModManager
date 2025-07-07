//
// Created by Adrien BLANCHET on 22/06/2020.
//

#include "FrameRoot.h"

#include <TabGames.h>
#include <TabAbout.h>
#include <TabGeneralSettings.h>


#include "Toolbox.h"

#include <AlchemistLogger.h>


FrameRoot::FrameRoot() {
  alchemyLogger.log("FrameRoot::FrameRoot();");

  this->setTitle("SimpleModManager");
  this->setFooterText( "v" + Toolbox::getAppVersion() );
  this->setIcon("romfs:/images/icon_corner.png");
  this->addTab( "Game Browser", new TabGames(this) );
  this->addSeparator();
  this->addTab( "Settings", new TabGeneralSettings(this) );
  this->addTab( "About", new TabAbout() );

}

bool FrameRoot::onCancel() {
  alchemyLogger.log("FrameRoot::onCancel();");
  // fetch the current focus
  auto* lastFocus = brls::Application::getCurrentFocus();

  // perform the cancel
  bool onCancel = TabFrame::onCancel();

  // if the focus is the same, then quit the app
  if( lastFocus == brls::Application::getCurrentFocus() ){ brls::Application::quit(); }

  return onCancel;
}
