#include "dialog_util.hpp"

/**
 * Builds a dialog to show for while the a longer process is running
 * 
 * @param action Short label describing the action being performed
 */
brls::Dialog* DialogUtil::buildLoadingDialog(std::string action) {
  auto* layout = new brls::BoxLayout(brls::BoxLayoutOrientation::HORIZONTAL);
  auto* label = new brls::Label(brls::LabelStyle::DIALOG, action + "...");
  auto* progress = new brls::ProgressSpinner();
  progress->willAppear(); // TODO: no progress spinner :(
  layout->addView(label);
  layout->addView(progress);

  auto* dialog = new brls::Dialog(layout);
  dialog->setCancelable(false);

  return dialog;
}