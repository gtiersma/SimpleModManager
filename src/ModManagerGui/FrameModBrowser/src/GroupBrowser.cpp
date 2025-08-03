//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#include "GroupBrowser.h"

#include <StateAlchemist/controller.h>


GroupBrowser::GroupBrowser(ModBrowser* mod_browser_) {
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

    item->getFocusEvent()->subscribe([mod_browser_, group](View* view) {
      if (controller.group == group) return; // Do nothing if group did not change
      controller.group = group;

      // Have mod browser load mods of the focused group:
      mod_browser_->loadFirstPage();
    });

    item->getClickEvent()->subscribe([mod_browser_](View* view) {
      mod_browser_->focusTop();
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

