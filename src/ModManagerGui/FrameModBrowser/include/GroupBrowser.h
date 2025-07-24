//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_GROUPBROWSER_H
#define SIMPLEMODMANAGER_GROUPBROWSER_H


#include "borealis.hpp"


class FrameModBrowser;

class GroupBrowser : public brls::List {

public:

  // Short width to provide more room for the mod list
  static const int WIDTH = 300;

  explicit GroupBrowser(FrameModBrowser* owner_);

  void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;

private:
  FrameModBrowser* _owner_{nullptr};
};

#endif //SIMPLEMODMANAGER_GROUPBROWSER_H
