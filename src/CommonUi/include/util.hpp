#ifndef SIMPLEMODMANAGER_UTIL_H
#define SIMPLEMODMANAGER_UTIL_H

#include "borealis.hpp"

namespace Util {
    void padTabContent(brls::Box* tabContent);

    brls::Dialog* buildLoadingDialog(std::string action);
}

#endif //SIMPLEMODMANAGER_UTIL_H