//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#include "GroupBrowser.h"
#include "ModBrowser.h"
#include "util.hpp"

#include <StateAlchemist/controller.h>


using namespace brls::literals;

/**
 * This is using a side bar directly just because having a second tab frame was crashing.
 * TODO: That could've been something else at the time. Try again?
 */
GroupBrowser::GroupBrowser() {
  this->inflateFromXMLRes("xml/FrameModBrowser/group_browser.xml");

  std::vector<std::string> groups = controller.loadGroups(true);

  for (std::string& group : groups) {
    this->groupList->addItem(group, [this, group](View* view) {
      // Only trigger when the sidebar item gains focus
      if (!view->isFocused())
        return;

      gameBrowser.getModManager().setGroup(group);
      
      // Remove the old group list before showing the new one
      // (if there is currently one shown).
      //
      // TODO: Would be a little safer if we were using an ID
      if (this->getChildren().size() == 2) {
        this->removeView(this->getChildren()[1]);
      }

      this->addView(new ModBrowser(view));
    });
  }
}

GroupBrowser* GroupBrowser::create() {
  return new GroupBrowser();
}