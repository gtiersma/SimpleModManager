//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "FrameModBrowser.h"

#include <TabModPlugins.h>
#include <TabModPresets.h>
#include <TabModOptions.h>

#include "GenericToolbox.Switch.h"
#include "Logger.h"
#include <StateAlchemist/controller.h>
#include <AlchemistLogger.h>


LoggerInit([]{
  Logger::setUserHeaderStr("[FrameModBrowser]");
});


FrameModBrowser::FrameModBrowser(GuiModManager* guiModManagerPtr_) : _guiModManagerPtr_(guiModManagerPtr_) {

  GameBrowser& gameBrowser = getGameBrowser();
  Game game = gameBrowser.getGame(controller.titleId).value();

  // fetch game title
  this->setTitle(game.name);

  std::string gamePath = controller.getGamePath();

  if (game.icon.size() > 0) { this->setIcon(game.icon.data(), 0x20000); }
  else { this->setIcon("romfs:/images/icon_corner.png"); }

  this->setFooterText("SimpleModManager");

  auto* parametersTabList = new brls::List();

  auto* presetParameter = new brls::ListItem("Config preset", "", "");
  presetParameter->setValue(gameBrowser.getConfigHandler().getConfig().getCurrentPresetName());
  parametersTabList->addView(presetParameter);

  _tabGroupBrowser_ = new TabGroupBrowser( this );
  _tabModPresets_ = new TabModPresets( this );
  _tabModOptions_ = new TabModOptions( this );
  _tabModPlugins_ = new TabModPlugins( this );

  _tabModOptions_->initialize();

  this->addTab("Mod Browser", _tabGroupBrowser_);
  this->addSeparator();
  this->addTab("Mod Presets", _tabModPresets_);
  this->addTab("Options", _tabModOptions_);
  this->addTab("Plugins", _tabModPlugins_);
}
bool FrameModBrowser::onCancel() {
  alchemyLogger.log("FrameModBrowser::onCancel();");

  // Go back to sidebar
  auto* lastFocus = brls::Application::getCurrentFocus();
  brls::Application::onGamepadButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT, false);

  // If the sidebar was already there, the focus has not changed
  if(lastFocus == brls::Application::getCurrentFocus()){
    LogInfo("Back on games screen...");
    brls::Application::popView(brls::ViewAnimation::SLIDE_RIGHT);
  }
  return true;

}

uint8_t *FrameModBrowser::getIcon() {
  return _icon_;
}
