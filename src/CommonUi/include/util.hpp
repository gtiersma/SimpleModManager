#ifndef SIMPLEMODMANAGER_UTIL_H
#define SIMPLEMODMANAGER_UTIL_H

#include "borealis.hpp"

/**
 * Just random UI shared functionality that needs to be performed often in many different places
 */
namespace Util {

    /**
     * Sets the box's padding to the amount that's commonly used for tab content.
     */
    void padTabContent(brls::Box* tabContent);

   /**
    * Builds a dialog to show for while the a longer process is running
    * 
    * @param action Short label describing the action being performed
    */
    brls::Dialog* buildLoadingDialog(std::string action);
}

#endif //SIMPLEMODMANAGER_UTIL_H