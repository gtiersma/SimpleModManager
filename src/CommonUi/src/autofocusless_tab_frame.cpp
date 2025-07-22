/*
    Borealis, a Nintendo Switch UI Library
    Copyright (C) 2019-2020  natinusala
    Copyright (C) 2019  WerWolv
    Copyright (C) 2019  p-sam

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    ---------------------------------------------------------------------------

    Custom version of UI control from Borealis

    Not part of the Borealis library. It's a modified copy of a Borealis control.
*/

#include <borealis/application.hpp>
#include <borealis/box_layout.hpp>
#include <borealis/rectangle.hpp>
#include <borealis/sidebar.hpp>

#include "autofocusless_tab_frame.hpp"

namespace brls
{

AutofocuslessTabFrame::AutofocuslessTabFrame()
    : AppletFrame(false, true)
{
    //Create sidebar
    this->sidebar = new Sidebar();

    // Setup content view
    this->layout = new BoxLayout(BoxLayoutOrientation::HORIZONTAL);
    layout->addView(sidebar);

    this->setContentView(layout);
}

bool AutofocuslessTabFrame::onCancel()
{
    // Go back to sidebar if not already focused
    if (!this->sidebar->isChildFocused())
    {
        Application::onGamepadButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT, false);
        return true;
    }

    return AppletFrame::onCancel();
}

void AutofocuslessTabFrame::switchToView(View* view)
{
    if (this->rightPane == view)
        return;

    if (this->layout->getViewsCount() > 1)
    {
        if (this->rightPane)
            this->rightPane->willDisappear(true);
        this->layout->removeView(1, false);
    }

    this->rightPane = view;
    if (this->rightPane != nullptr)
        this->layout->addView(this->rightPane, true, true); // addView() calls willAppear()
}

void AutofocuslessTabFrame::addTab(std::string label, View* view)
{
    SidebarItem* item = this->sidebar->addItem(label, view);
    item->getFocusEvent()->subscribe([this](View* view) {
        if (SidebarItem* item = dynamic_cast<SidebarItem*>(view))
            this->switchToView(item->getAssociatedView());
    });
}

void AutofocuslessTabFrame::addSeparator()
{
    this->sidebar->addSeparator();
}

View* AutofocuslessTabFrame::getDefaultFocus()
{
    // Try to focus the right pane
    if (this->layout->getViewsCount() > 1)
    {
        View* newFocus = this->rightPane->getDefaultFocus();

        if (newFocus)
            return newFocus;
    }

    // Otherwise focus sidebar
    return this->sidebar->getDefaultFocus();
}

AutofocuslessTabFrame::~AutofocuslessTabFrame()
{
    switchToView(nullptr);

    // Content view is freed by ~AppletFrame()
}

} // namespace brls
