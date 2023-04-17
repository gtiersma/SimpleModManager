//
// Created by Nadrino on 06/06/2020.
//

#include "GlobalObjects.h"
#include "Toolbox.h"

#include "GenericToolbox.h"

namespace GlobalObjects{

  static bool _quit_now_triggered_ = false;
  std::string _version_str_ = "v" + Toolbox::get_app_version();
  PadState gPad{};

  ModBrowser &getModBrowser() {
    return _mod_browser_;
  }

  void setTriggerSwitchUI(bool triggerSwitchUI_){
    GlobalObjects::_triggerSwitchUI_ = std::to_string(triggerSwitchUI_);
  }

  bool doTriggerSwitchUI(){
    return GenericToolbox::toBool(GlobalObjects::_triggerSwitchUI_);
  }

  void set_quit_now_triggered(bool value_){
    GlobalObjects::_quit_now_triggered_ = value_;
  }

  bool is_quit_now_triggered(){
    return GlobalObjects::_quit_now_triggered_;
  }

}
