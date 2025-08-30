/*
    Copyright 2019-2021 natinusala
    Copyright 2019 WerWolv
    Copyright 2019 p-sam

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <borealis.hpp>
#include <focus_sidebar.hpp>

using namespace brls::literals;

namespace brls
{

FocusSidebar::FocusSidebar()
{
    Style style = Application::getStyle();

    this->setScrollingBehavior(ScrollingBehavior::CENTERED);
    this->setBackground(ViewBackground::SIDEBAR);

    // Create content box
    this->contentBox = new Box(Axis::COLUMN);

    this->contentBox->setPadding(
        style["brls/sidebar/padding_top"],
        style["brls/sidebar/padding_right"],
        style["brls/sidebar/padding_bottom"],
        style["brls/sidebar/padding_left"]);

    this->setContentView(this->contentBox);
    this->setScrollingIndicatorVisible(false);
}

void FocusSidebar::addItem(std::string label, GenericEvent::Callback focusCallback)
{
    SidebarItem* item = new SidebarItem();
    item->setGroup(&this->group);
    item->setLabel(label);
    item->getFocusEvent()->subscribe(focusCallback);

    this->contentBox->addView(item);
}

SidebarItem* FocusSidebar::getItem(int position)
{
    return dynamic_cast<SidebarItem*>(this->contentBox->getChildren()[position]);
}

void FocusSidebar::addSeparator()
{
    this->contentBox->addView(new SidebarSeparator());
}

void FocusSidebar::clearItems()
{
    this->contentBox->clearViews();
    group.clear();
}

View* FocusSidebar::create()
{
    return new FocusSidebar();
}

} // namespace brls
