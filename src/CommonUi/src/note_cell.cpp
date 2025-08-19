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
*/

#include "note_cell.hpp"

namespace brls
{

const std::string noteCellXML = R"xml(
<brls:Box
    axis="column"
    width="auto"
    height="140.0"
    focusable="true"
    paddingTop="12.5"
    paddingBottom="12.5">

    <brls:Label
        id="brls/notecell/title"
        width="auto"
        height="auto"
        grow="1.0"
        fontSize="@style/brls/sidebar/item_font_size"
        marginTop="@style/brls/sidebar/item_accent_margin_top_bottom"
        marginBottom="6.0"
        marginLeft="@style/brls/sidebar/item_accent_margin_sides"
        marginRight="@style/brls/sidebar/item_accent_margin_sides" />

    <brls:Label
        id="brls/notecell/note"
        width="auto"
        height="auto"
        grow="1.0"
        fontSize="18.0"
        textColor="@theme/brls/header/subtitle"
        marginTop="6.0"
        marginBottom="@style/brls/sidebar/item_accent_margin_top_bottom"
        marginLeft="@style/brls/sidebar/item_accent_margin_sides"
        marginRight="@style/brls/sidebar/item_accent_margin_sides" />

</brls:Box>
)xml";

NoteCell::NoteCell()
{
    this->inflateFromXMLString(noteCellXML);

    this->registerStringXMLAttribute("title", [this](std::string value)
        { this->title->setText(value); });
    this->registerStringXMLAttribute("note", [this](std::string value)
        { this->note->setText(value); });
}

void NoteCell::setText(std::string title)
{
    this->title->setText(title);
}

void NoteCell::setTextColor(NVGcolor color)
{
    this->title->setTextColor(color);
}

void NoteCell::setNote(std::string note)
{
    this->note->setText(note);
}

View* NoteCell::create()
{
    return new NoteCell();
}

} // namespace brls
