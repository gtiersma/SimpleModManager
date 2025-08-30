/*
    Copyright 2019-2021 natinusala
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

#pragma once

#include <borealis.hpp>

namespace brls
{

class FocusSidebar : public ScrollingFrame
{
  public:
    FocusSidebar();

    /**
     * Adds an item to this sidebar. The given callback will be called
     * when the item becomes active.
     */
    void addItem(std::string label, GenericEvent::Callback focusCallback);

    SidebarItem* getItem(int position);

    /**
     * Adds a separator to this sidebar.
     */
    void addSeparator();

    /**
     * Removes all items from sidebar.
     */
    void clearItems();

    static View* create();

  private:
    SidebarItemGroup group;

    Box* contentBox;
};

} // namespace brls
