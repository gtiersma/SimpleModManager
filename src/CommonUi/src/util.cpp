#include "util.hpp"

void Util::padTabContent(brls::Box* tabContent) {
  brls::Style style = brls::Application::getStyle();
  tabContent->setPadding(
    style["brls/tab_details/padding_top"],
    style["brls/tab_details/padding_right"],
    style["brls/tab_details/padding_bottom"],
    style["brls/tab_details/padding_left"]
  );
}

/**
 * Builds a dialog to show for while the a longer process is running
 * 
 * @param action Short label describing the action being performed
 */
brls::Dialog* Util::buildLoadingDialog(std::string action) {
  brls::Box* container = new brls::Box(brls::Axis::COLUMN);

  brls::Label* label = new brls::Label();
  label->setText(action + "...");
  container->addView(label);

  brls::ProgressSpinner* progress = new brls::ProgressSpinner();
  container->addView(progress);

  brls::Dialog* dialog = new brls::Dialog(container);
  dialog->setCancelable(false);

  return dialog;
}