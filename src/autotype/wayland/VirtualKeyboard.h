/*
 *  Copyright (C) 2016 Lennart Glauer <mail@lennart-glauer.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <memory>

#include "UInput.h"
#include "XKBMap.h"

class VirtualKeyboard
{
public:
    VirtualKeyboard();
    ~VirtualKeyboard() = default;

    bool init(const xkb_rule_names* xkbRuleNames);
    bool sendKey(const uint32_t keycode, const bool isKeyDown) const;
    bool sendChar(const uint32_t utf32, const bool isKeyDown) const;

private:
    std::unique_ptr<UInput> m_uInput;
    std::unique_ptr<XKBMap> m_xkbMap;
};

#endif  // KEYBOARD_H_
