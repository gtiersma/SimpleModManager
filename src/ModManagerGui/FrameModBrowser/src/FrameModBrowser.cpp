//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include "FrameModBrowser.h"

#include <TabModPresets.h>
#include <TabModOptions.h>

#include "GenericToolbox.Switch.h"
#include "Logger.h"
#include <StateAlchemist/controller.h>


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

  this->setFooterText("Simple Mod Alchemist");

  _groupBrowser_ = new GroupBrowser(this);
  _modBrowser_ = new ModBrowser(this);

  _tabModBrowser_ = new brls::BoxLayout(brls::BoxLayoutOrientation::HORIZONTAL);
  _tabModBrowser_->addView(_groupBrowser_);
  _tabModBrowser_->addView(_modBrowser_, true);

  //_tabModPresets_ = new TabModPresets(this);
  _tabModOptions_ = new TabModOptions(this);

  _tabModOptions_->initialize();

  this->addTab("Mod Browser", _tabModBrowser_);
  this->addSeparator();
  //this->addTab("Mod Presets", _tabModPresets_);
  this->addTab("Options", _tabModOptions_);
}

void FrameModBrowser::handleGroupSelect() {
  _modBrowser_->loadFirstPage();
}

bool FrameModBrowser::onCancel() {

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
