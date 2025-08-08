//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#include "GroupBrowser.h"
#include "ModBrowser.h"

#include <StateAlchemist/controller.h>


GroupBrowser::GroupBrowser() {
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
    this->addTab(group, []() { return new ModBrowser(); });
  }
}