#include "dialog_util.hpp"

/**
 * Builds a dialog to show for while the a longer process is running
 * 
 * @param action Short label describing the action being performed
 */
brls::Dialog* DialogUtil::buildLoadingDialog(std::string action) {
  brls::Box* container = new brls::Box();

  brls::Label* label = new brls::Label();
  label->setText(action + "...");
  container->addView(label);

  brls::ProgressSpinner* progress = new brls::ProgressSpinner();
  container->addView(progress);

  brls::Dialog* dialog = new brls::Dialog(container);
  dialog->setCancelable(false);

  return dialog;
}