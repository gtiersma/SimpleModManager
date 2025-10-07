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
     * Builds a dialog that can be shown that gets confirmation before doing something, showing a loading spinner while doing it
     * 
     * @param warning Short message warning the user of what will happen
     * @param action Shorter message describing the action being performed in present tense
     * @param task The functionality that performs the action
     * @param finishedCallback Optional; functionality to perform after the action completes
     */
    brls::Dialog* buildConfirmDialog(
        const std::string& warning,
        const std::string& action,
        std::function<void()> task,
        std::function<void()> finishedCallback = []() {}
    );

   /**
    * Builds a dialog to show for while the a longer process is running
    * 
    * @param action Short label describing the action being performed
    */
    brls::Dialog* buildLoadingDialog(std::string action);
}

#endif //SIMPLEMODMANAGER_UTIL_H