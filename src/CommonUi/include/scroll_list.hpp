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

namespace brls
{

class ScrollList; // forward declaration

// The content view of lists (used internally)
class ScrollListContentView : public BoxLayout
{
  public:
    ScrollListContentView(ScrollList* list, size_t defaultFocus = 0);
    
  protected:
    void customSpacing(View* current, View* next, int* spacing) override;
    
  private:
    ScrollList* list;
};

class ScrollList : public View
{
  private:
    ScrollListContentView* listLayout;
    View* contentView = nullptr;

    bool ready = false; // has layout been called at least once?

    unsigned middleY = 0; // y + height/2
    unsigned bottomY = 0; // y + height

    float scrollY = 0.0f; // from 0.0f to 1.0f, in % of content view height

    float scrollBarAlpha = Application::getTheme()->scrollBarAlphaNormal;

    bool updateScrollingOnNextLayout = false;
    bool updateScrollingOnNextFrame  = false;

    unsigned getYCenter(View* view);

    void prebakeScrolling();
    bool updateScrolling(bool animated);
    void scrollAnimationTick();

  public:
    ScrollList(size_t defaultFocus = 0);
    ~ScrollList();

    float scrollOverride = -1.0f;

    // Wrapped BoxLayout methods
    void addView(View* view, bool fill = false);
    void removeView(int index, bool free = true);
    void clear(bool free = true);
    size_t getViewsCount();
    View* getChild(size_t i);
    void setMargins(unsigned top, unsigned right, unsigned bottom, unsigned left);
    void setMarginBottom(unsigned bottom);
    void setSpacing(unsigned spacing);
    unsigned getSpacing();
    virtual void customSpacing(View* current, View* next, int* spacing);

    void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, Style* style, FrameContext* ctx) override;
    void layout(NVGcontext* vg, Style* style, FontStash* stash) override;
    void willAppear(bool resetState = false) override;
    void willDisappear(bool resetState = false) override;
    View* getDefaultFocus() override;
    void onChildFocusGained(View* child) override;
    void onWindowSizeChanged() override;

    void setContentView(View* view);
    View* getContentView();

    void refresh();
    
    void startScrolling(bool animated, float newScroll);
};

}