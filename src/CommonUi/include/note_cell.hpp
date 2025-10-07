/*
    Copyright 2021 XITRIX

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    -----

    Not an official file that's part of the Borealis library,
    but rather a direct copy with minimal modifications,
    so the license above still applies.
*/

#pragma once

#include <borealis/core/application.hpp>
#include <borealis/core/bind.hpp>
#include <borealis/core/box.hpp>
#include <borealis/views/recycler.hpp>
#include <borealis/views/image.hpp>

namespace brls
{

// A cell that can include a description in it.
// Useful when the action it performs requires additional explanation or clarity.
class NoteCell : public RecyclerCell
{
  public:
    NoteCell();

    void setText(std::string title);
    void setTextColor(NVGcolor color);

    void setNote(std::string note);

    BRLS_BIND(Label, note, "brls/notecell/note");
    BRLS_BIND(Label, title, "brls/notecell/title");

    static View* create();
};

} // namespace brls
