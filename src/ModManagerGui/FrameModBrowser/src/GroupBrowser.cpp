//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#include "GroupBrowser.h"
#include "FrameModBrowser.h"

#include <StateAlchemist/controller.h>


GroupBrowser::GroupBrowser(FrameModBrowser* owner_) : _owner_(owner_) {
  this->setWidth(this->WIDTH);

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
    auto* item = new brls::ListItem(group);

    item->getFocusEvent()->subscribe([owner_, item](View* view) {
      // Pass the selected group to FrameModBrowser so it can pass it to the ModBrowser
      owner_->handleGroupSelect(item->getLabel());
    });

    this->addView(item);
  }
}

void GroupBrowser::draw(
  NVGcontext *vg,
  int x,
  int y,
  unsigned int width,
  unsigned int height,
  brls::Style *style,
  brls::FrameContext *ctx
) {
  ScrollView::draw(vg, x, y, width, height, style, ctx);
}

