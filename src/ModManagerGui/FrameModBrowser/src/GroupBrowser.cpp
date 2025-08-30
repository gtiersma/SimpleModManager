//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#include "GroupBrowser.h"
#include "ModBrowser.h"
#include "util.hpp"

#include <StateAlchemist/controller.h>


GroupBrowser::GroupBrowser() {
  this->inflateFromXMLRes("xml/FrameModBrowser/group_browser.xml");

  std::vector<std::string> groups = controller.loadGroups(true);

  if (groups.empty()) {
    brls::Dialog* dialog = new brls::Dialog(
      "No mod groups have been found in " + controller.getGamePath() +
      ". Within that folder, organize the mods in this manner: ./<group>/<thing-being-replaced>/<mod-name>/<file-structure-in-installed-directory>"
    );
    dialog->open();
    return;
  }

  for (std::string& group : groups) {
    this->groupList->addItem(group, [this, group](View* view) {
      gameBrowser.getModManager().setGroup(group);
      
      if (this->getChildren().size() == 2) {
        this->removeView(this->getChildren()[1]);
      }

      this->addView(new ModBrowser());
    });
  }
}

GroupBrowser* GroupBrowser::create() {
  return new GroupBrowser();
}