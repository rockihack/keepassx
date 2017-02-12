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

#include "XKBMap.h"

#include <linux/input-event-codes.h>

#define LINUX_TO_XKB_KEYCODE(keycode) ((keycode) + 8)
#define XKB_TO_LINUX_KEYCODE(keycode) ((keycode) - 8)

XKBMap::XKBMap()
    : m_initialized(false)
{
}

//
// Initialize xkb map.
//
bool XKBMap::init(const xkb_rule_names* xkbRuleNames)
{
    if (m_initialized) {
        return true;
    }

    if (!buildMap(xkbRuleNames)) {
        return false;
    }

    m_initialized = true;
    return true;
}

//
// Lookup unicode character in utf32 format
// and return corresponding keyboard state
//
bool XKBMap::lookupChar(const uint32_t utf32, KeyboardState& state) const
{
    if (!m_initialized) {
        return false;
    }

    std::unordered_map<uint32_t, KeyboardState>::const_iterator it = m_unicodeMap.find(utf32);
    if (it == m_unicodeMap.end()) {
        return false;
    }

    state = it->second;
    return true;
}

//
// Build unicode (utf32) to keyboard state map.
//
bool XKBMap::buildMap(const xkb_rule_names* xkbRuleNames)
{
    xkb_context* xkbContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (xkbContext == nullptr) {
        return false;
    }
    xkb_keymap* xkbKeymap = xkb_keymap_new_from_names(xkbContext, xkbRuleNames, XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (xkbKeymap == nullptr) {
        xkb_context_unref(xkbContext);
        return false;
    }
    xkb_state* xkbState = xkb_state_new(xkbKeymap);
    if (xkbState == nullptr) {
        xkb_keymap_unref(xkbKeymap);
        xkb_context_unref(xkbContext);
        return false;
    }

    const uint32_t modifierMasks[] = {
        MOD_ALTGR | MOD_SHIFT,
        MOD_ALTGR,
        MOD_SHIFT,
        NONE
    };
    const xkb_keycode_t minKeycode = xkb_keymap_min_keycode(xkbKeymap);
    const xkb_keycode_t maxKeycode = xkb_keymap_max_keycode(xkbKeymap);

    // Enumerate all xkb keyboard states and build unicode to keyboard state map
    for (size_t i = 0; i < sizeof(modifierMasks) / sizeof(modifierMasks[0]); ++i) {
        if (modifierMasks[i] & MOD_SHIFT) {
            xkb_state_update_key(xkbState, LINUX_TO_XKB_KEYCODE(KEY_LEFTSHIFT), XKB_KEY_DOWN);
        }
        if (modifierMasks[i] & MOD_ALTGR) {
            xkb_state_update_key(xkbState, LINUX_TO_XKB_KEYCODE(KEY_RIGHTALT), XKB_KEY_DOWN);
        }

        for (xkb_keycode_t keycode = minKeycode; keycode <= maxKeycode; ++keycode) {
            const uint32_t utf32 = xkb_state_key_get_utf32(xkbState, keycode);

            if (utf32 != 0 && keycode >= 8) {
                m_unicodeMap[utf32] = KeyboardState {
                    XKB_TO_LINUX_KEYCODE(keycode), modifierMasks[i]
                };
            }
        }

        if (modifierMasks[i] & MOD_ALTGR) {
            xkb_state_update_key(xkbState, LINUX_TO_XKB_KEYCODE(KEY_RIGHTALT), XKB_KEY_UP);
        }
        if (modifierMasks[i] & MOD_SHIFT) {
            xkb_state_update_key(xkbState, LINUX_TO_XKB_KEYCODE(KEY_LEFTSHIFT), XKB_KEY_UP);
        }
    }

    xkb_state_unref(xkbState);
    xkb_keymap_unref(xkbKeymap);
    xkb_context_unref(xkbContext);
    return true;
}
