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

#include "VirtualKeyboard.h"

#include <linux/input-event-codes.h>

VirtualKeyboard::VirtualKeyboard()
    : m_uInput(new UInput())
    , m_xkbMap(new XKBMap())
{
}

//
// Initialize virtual keyboard
//
bool VirtualKeyboard::init(const xkb_rule_names* xkbRuleNames)
{
    return m_uInput->init() && m_xkbMap->init(xkbRuleNames);
}

//
// Send keycode
//
bool VirtualKeyboard::sendKey(const uint32_t keycode, const bool isKeyDown) const
{
    const UInput::KeyState keyState = isKeyDown ? UInput::Keypress : UInput::Release;
    return m_uInput->sendKey(keycode, keyState);
}

//
// Send unicode character in utf32 format
//
bool VirtualKeyboard::sendChar(const uint32_t utf32, const bool isKeyDown) const
{
    XKBMap::KeyboardState keyBoardState;

    // Lookup keycode and modifiers
    if (!m_xkbMap->lookupChar(utf32, keyBoardState)) {
        return false;
    }

    // Press modifiers
    if (isKeyDown) {
        if ((keyBoardState.modmask & XKBMap::MOD_SHIFT)
                && !sendKey(KEY_LEFTSHIFT, true)) {
            return false;
        }
        if ((keyBoardState.modmask & XKBMap::MOD_ALTGR)
                && !sendKey(KEY_RIGHTALT, true)) {
            return false;
        }
    }

    // Press or release key
    if (!sendKey(keyBoardState.keycode, isKeyDown)) {
        return false;
    }

    // Release modifiers
    if (!isKeyDown) {
        if ((keyBoardState.modmask & XKBMap::MOD_ALTGR)
                && !sendKey(KEY_RIGHTALT, false)) {
            return false;
        }
        if ((keyBoardState.modmask & XKBMap::MOD_SHIFT)
                && !sendKey(KEY_LEFTSHIFT, false)) {
            return false;
        }
    }

    return true;
}
