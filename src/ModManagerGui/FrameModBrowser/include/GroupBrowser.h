//
// Derived from a file created by Adrien BLANCHET on 21/06/2020.
//

#ifndef SIMPLEMODMANAGER_GROUPBROWSER_H
#define SIMPLEMODMANAGER_GROUPBROWSER_H


#include "borealis.hpp"


class GroupBrowser : public brls::ScrollingFrame {
  public:
    explicit GroupBrowser(FrameModBrowser* owner_);
};

#endif //SIMPLEMODMANAGER_GROUPBROWSER_H
