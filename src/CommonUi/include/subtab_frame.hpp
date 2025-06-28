/*
    Borealis, a Nintendo Switch UI Library
    Copyright (C) 2019-2020  natinusala
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

#pragma once

#include "borealis.hpp"
#include <string>
#include <vector>

namespace brls
{

// An applet frame containing a sidebar on the left with multiple tabs
class SubtabFrame : public View
{
  public:
    SubtabFrame();

    /**
     * Adds a tab with given label and view
     * All tabs and separators must be added
     * before the TabFrame is itself added to
     * the view hierarchy
     */
    void addTab(std::string label, View* view);
    void addSeparator();

    View* getDefaultFocus() override;

    virtual bool onCancel();

    void setContentView(View* view);

    void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, Style* style, FrameContext* ctx) override;

    void show(std::function<void(void)> cb, bool animate = true, ViewAnimation animation = ViewAnimation::FADE) override;
    void hide(std::function<void(void)> cb, bool animated = true, ViewAnimation animation = ViewAnimation::FADE) override;

    ~SubtabFrame();

    Sidebar* sidebar;

  protected:
    void layout(NVGcontext* vg, Style* style, FontStash* stash) override;

  private:
    BoxLayout* layout;
    View* rightPane = nullptr;

    View* contentView = nullptr;

    bool slideOut = false;
    bool slideIn  = false;

    ViewAnimation animation;

    void switchToView(View* view);
};

} // namespace brls