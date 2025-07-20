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

#include "borealis.hpp"
#include "subtab_frame.hpp"

using namespace brls::i18n::literals;

namespace brls
{

SubtabFrame::SubtabFrame()
{
    //Create sidebar
    this->sidebar = new Sidebar();
    this->sidebar->setWidth(this->SIDE_BAR_WIDTH);

    // Setup content view
    this->boxLayout = new BoxLayout(BoxLayoutOrientation::HORIZONTAL);
    boxLayout->addView(sidebar);

    this->setContentView(boxLayout);

    this->registerAction("brls/hints/back"_i18n, Key::B, [this] { return this->onCancel(); });
}

bool SubtabFrame::onCancel()
{
    // Go back to sidebar if not already focused
    if (!this->sidebar->isChildFocused())
    {
        Application::onGamepadButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT, false);
        return true;
    }

    Application::popView();
    return true;
}

void SubtabFrame::switchToView(View* view)
{
    if (this->rightPane == view)
        return;

    if (this->boxLayout->getViewsCount() > 1)
    {
        if (this->rightPane)
            this->rightPane->willDisappear(true);
        this->boxLayout->removeView(1, false);
    }

    this->rightPane = view;
    if (this->rightPane != nullptr)
        this->boxLayout->addView(this->rightPane, true, true); // addView() calls willAppear()
}

void SubtabFrame::addTab(std::string label, View* view)
{
    SidebarItem* item = this->sidebar->addItem(label, view);
    item->getFocusEvent()->subscribe([this](View* view) {
        if (SidebarItem* item = dynamic_cast<SidebarItem*>(view))
            this->switchToView(item->getAssociatedView());
    });
}

void SubtabFrame::addSeparator()
{
    this->sidebar->addSeparator();
}

View* SubtabFrame::getDefaultFocus()
{
    // Try to focus the right pane
    if (this->boxLayout->getViewsCount() > 1)
    {
        View* newFocus = this->rightPane->getDefaultFocus();

        if (newFocus)
            return newFocus;
    }

    // Otherwise focus sidebar
    return this->sidebar->getDefaultFocus();
}

void SubtabFrame::setContentView(View* view)
{
    this->contentView = view;

    if (this->contentView)
    {
        this->contentView->setParent(this);
        this->contentView->willAppear();
    }

    this->invalidate();
}

void SubtabFrame::layout(NVGcontext* vg, Style* style, FontStash* stash)
{
    this->contentView->setBoundaries(this->x, this->y, this->width, this->height);
    this->contentView->invalidate();
}

void SubtabFrame::draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, Style* style, FrameContext* ctx)
{
    // Separators
    nvgFillColor(vg, a(ctx->theme->separatorColor));

    // Content view
    if (contentView)
    {
        float slideAlpha = 1.0f - this->contentView->alpha;

        if ((this->slideIn && this->animation == ViewAnimation::SLIDE_LEFT) || (this->slideOut && this->animation == ViewAnimation::SLIDE_RIGHT))
            slideAlpha = 1.0f - slideAlpha;

        int translation = (int)((float)style->AppletFrame.slideAnimation * slideAlpha);

        if ((this->slideIn && this->animation == ViewAnimation::SLIDE_LEFT) || (this->slideOut && this->animation == ViewAnimation::SLIDE_RIGHT))
            translation -= style->AppletFrame.slideAnimation;

        if (this->slideOut || this->slideIn)
            nvgTranslate(vg, -translation, 0);

        contentView->frame(ctx);

        if (this->slideOut || this->slideIn)
            nvgTranslate(vg, translation, 0);
    }
}

void SubtabFrame::show(std::function<void(void)> cb, bool animated, ViewAnimation animation)
{
    this->animation = animation;

    if (animated && (animation == ViewAnimation::SLIDE_LEFT || animation == ViewAnimation::SLIDE_RIGHT) && this->contentView)
    {
        this->slideIn = true;

        this->contentView->show([this]() {
            this->slideIn = false;
        },
            true, animation);
    }
    else if (this->contentView && this->contentView->isHidden())
    {
        this->contentView->show([]() {}, animated, animation);
    }

    View::show(cb, animated, animation);
}

void SubtabFrame::hide(std::function<void(void)> cb, bool animated, ViewAnimation animation)
{
    this->animation = animation;

    if (animated && (animation == ViewAnimation::SLIDE_LEFT || animation == ViewAnimation::SLIDE_RIGHT) && this->contentView)
    {
        this->slideOut = true;

        this->contentView->hide([this, cb]() {
            this->slideOut = false;
        },
            true, animation);
    }
    else if (this->contentView && !this->contentView->isHidden())
    {
        this->contentView->hide([]() {}, animated, animation);
    }

    View::hide(cb, animated, animation);
}

SubtabFrame::~SubtabFrame()
{
    switchToView(nullptr);

    if (this->contentView)
    {
        this->contentView->willDisappear(true);
        delete this->contentView;
    }
}

} // namespace brls
