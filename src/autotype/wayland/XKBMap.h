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

#ifndef XKB_MAP_H_
#define XKB_MAP_H_

#include <unordered_map>
#include <xkbcommon/xkbcommon.h>

class XKBMap
{
public:
    struct KeyboardState
    {
        uint32_t keycode;
        uint32_t modmask;
    };
    enum KeyboardModifiers
    {
        NONE        = 0,
        MOD_SHIFT   = (1 << 0),
        MOD_ALTGR   = (1 << 1)
    };

    XKBMap();
    virtual ~XKBMap() = default;

    bool init(const xkb_rule_names* xkbRuleNames);
    bool lookupChar(const uint32_t utf32, KeyboardState& state) const;

private:
    bool m_initialized;
    std::unordered_map<uint32_t, KeyboardState> m_unicodeMap;

    bool buildMap(const xkb_rule_names* xkbRuleNames);
};

#endif  // XKB_MAP_H_
