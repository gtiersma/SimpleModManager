//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#include "TabGroupBrowser.h"
#include "FrameModBrowser.h"
#include "GenericToolbox.Macro.h"

#include <future>
#include <StateAlchemist/controller.h>


TabGroupBrowser::TabGroupBrowser(FrameModBrowser* owner_) : _owner_(owner_) {

  // Use subtab-sized sidebar to leave more room for mods
  this->sidebar->setWidth(SubtabFrame::SIDE_BAR_WIDTH);

  // Fetch the available groups
  auto groupList = controller.loadGroups(true);

  if (groupList.empty()) {
    auto dialog = new brls::Dialog(
      "No mod groups have been found in " + controller.getGamePath() +
      ". Within that folder, organize the mods in this manner: ./<group>/<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
    );
    dialog->open();
    return;
  }

  for (auto& group : groupList) {
    this->addTab(group, new TabModBrowser(_owner_, group));
  }
}

bool TabGroupBrowser::onCancel() {

  // Go back to sidebar
  auto* lastFocus = brls::Application::getCurrentFocus();
  brls::Application::onGamepadButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT, false);

  // If the sidebar was already there, the focus has not changed
  if(lastFocus == brls::Application::getCurrentFocus()){
    brls::Application::popView(brls::ViewAnimation::SLIDE_RIGHT);
  }
  return true;

}
