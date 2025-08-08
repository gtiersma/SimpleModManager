//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#include "GroupBrowser.h"
#include "ModBrowser.h"

#include <StateAlchemist/controller.h>


GroupBrowser::GroupBrowser(FrameModBrowser* owner_) {
  std::vector<std::string> groupList = controller.loadGroups(true);

  if (groupList.empty()) {
    brls::Dialog* dialog = new brls::Dialog(
      "No mod groups have been found in " + controller.getGamePath() +
      ". Within that folder, organize the mods in this manner: ./<group>/<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
    );
    dialog->open();
    return;
  }

  brls::TabFrame* tabs = new brls::TabFrame();
  for (std::string& group : groupList) {
    tabs->addTab(group, [owner_, group]() {
      controller.group = group;
      return new ModBrowser(owner_);
    });
  }
  this->addView(tabs);
}