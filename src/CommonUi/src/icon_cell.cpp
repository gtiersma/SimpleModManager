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

#include "icon_cell.hpp"

namespace brls
{

const std::string detailCellXML = R"xml(
<brls:Box
    width="auto"
    height="auto"
    focusable="true"
    paddingTop="12.5"
    paddingBottom="12.5">

    <brls:Image
        id="brls/iconcell/image"
        width="44px"
        height="44px"
        marginLeft="@style/brls/sidebar/item_accent_margin_sides"
        marginRight="@style/brls/sidebar/item_accent_margin_sides"/>

    <brls:Label
        id="brls/iconcell/title"
        width="auto"
        height="auto"
        grow="1.0"
        fontSize="@style/brls/sidebar/item_font_size"
        marginTop="@style/brls/sidebar/item_accent_margin_top_bottom"
        marginBottom="@style/brls/sidebar/item_accent_margin_top_bottom"
        marginLeft="@style/brls/sidebar/item_accent_margin_sides"
        marginRight="@style/brls/sidebar/item_accent_margin_sides" />

</brls:Box>
)xml";

IconCell::IconCell()
{
    this->inflateFromXMLString(detailCellXML);

    this->registerStringXMLAttribute("title", [this](std::string value)
        { this->title->setText(value); });
}

void IconCell::setIconFromMem(const unsigned char *data, int size)
{
    this->image->setImageFromMem(data, size);
}

void IconCell::setText(std::string title)
{
    this->title->setText(title);
}

void IconCell::setTextColor(NVGcolor color)
{
    this->title->setTextColor(color);
}

View* IconCell::create()
{
    return new IconCell();
}

} // namespace brls
