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

#include "AutoTypeWayland.h"

#include <unistd.h>
#include <linux/input-event-codes.h>

#define INVALID_KEYCODE 0xffffffff

AutoTypePlatformWayland::AutoTypePlatformWayland()
    : m_virtualKeyboard(new VirtualKeyboard())
{
}

bool AutoTypePlatformWayland::isAvailable()
{
    //const xkb_rule_names xkbRuleNames = { "evdev", "pc105", "us", "", "" };
    return m_virtualKeyboard->init(nullptr);
}

//
// Not implemented
//
QStringList AutoTypePlatformWayland::windowTitles()
{
    return QStringList();
}

//
// Not implemented
//
WId AutoTypePlatformWayland::activeWindow()
{
    return -1;
}

//
// Not implemented
//
QString AutoTypePlatformWayland::activeWindowTitle()
{
    return QString();
}

//
// Not implemented
//
bool AutoTypePlatformWayland::registerGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(key);
    Q_UNUSED(modifiers);

    return false;
}

//
// Not implemented
//
void AutoTypePlatformWayland::unregisterGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(key);
    Q_UNUSED(modifiers);
}

//
// Not implemented
//
int AutoTypePlatformWayland::platformEventFilter(void* event)
{
    Q_UNUSED(event);
    Q_ASSERT(false);

    return -1;
}

AutoTypeExecutor* AutoTypePlatformWayland::createExecutor()
{
    return new AutoTypeExecutorWayland(this);
}

int AutoTypePlatformWayland::initialTimeout()
{
    return 2000;
}

//
// Not implemented
//
bool AutoTypePlatformWayland::raiseWindow(WId window)
{
    Q_UNUSED(window);

    return false;
}

void AutoTypePlatformWayland::sendChar(const QChar& ch, const bool isKeyDown) const
{
    m_virtualKeyboard->sendChar(ch.unicode(), isKeyDown);
}

void AutoTypePlatformWayland::sendKey(Qt::Key key, const bool isKeyDown) const
{
    uint32_t nativeKeycode = qtToNativeKeyCode(key);
    if (nativeKeycode != INVALID_KEYCODE) {
        m_virtualKeyboard->sendKey(nativeKeycode, isKeyDown);
    }
}

uint32_t AutoTypePlatformWayland::qtToNativeKeyCode(Qt::Key key)
{
    switch (key) {
    case Qt::Key_Escape:
        return KEY_ESC;

    case Qt::Key_1:
        return KEY_1;
    case Qt::Key_2:
        return KEY_2;
    case Qt::Key_3:
        return KEY_3;
    case Qt::Key_4:
        return KEY_4;
    case Qt::Key_5:
        return KEY_5;
    case Qt::Key_6:
        return KEY_6;
    case Qt::Key_7:
        return KEY_7;
    case Qt::Key_8:
        return KEY_8;
    case Qt::Key_9:
        return KEY_9;
    case Qt::Key_0:
        return KEY_0;

    case Qt::Key_Backspace:
        return KEY_BACKSPACE;
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
        return KEY_TAB;

    case Qt::Key_Q:
        return KEY_Q;
    case Qt::Key_W:
        return KEY_W;
    case Qt::Key_E:
        return KEY_E;
    case Qt::Key_R:
        return KEY_R;
    case Qt::Key_T:
        return KEY_T;
    case Qt::Key_Y:
        return KEY_Y;
    case Qt::Key_U:
        return KEY_U;
    case Qt::Key_I:
        return KEY_I;
    case Qt::Key_O:
        return KEY_O;
    case Qt::Key_P:
        return KEY_P;

    case Qt::Key_Enter:
    case Qt::Key_Return:
        return KEY_ENTER;

    case Qt::Key_A:
        return KEY_A;
    case Qt::Key_S:
        return KEY_S;
    case Qt::Key_D:
        return KEY_D;
    case Qt::Key_F:
        return KEY_F;
    case Qt::Key_G:
        return KEY_G;
    case Qt::Key_H:
        return KEY_H;
    case Qt::Key_J:
        return KEY_J;
    case Qt::Key_K:
        return KEY_K;
    case Qt::Key_L:
        return KEY_L;

    case Qt::Key_Z:
        return KEY_Z;
    case Qt::Key_X:
        return KEY_X;
    case Qt::Key_C:
        return KEY_C;
    case Qt::Key_V:
        return KEY_V;
    case Qt::Key_B:
        return KEY_B;
    case Qt::Key_N:
        return KEY_N;
    case Qt::Key_M:
        return KEY_M;

    case Qt::Key_Space:
        return KEY_SPACE;
    case Qt::Key_CapsLock:
        return KEY_CAPSLOCK;

    case Qt::Key_NumLock:
        return KEY_NUMLOCK;
    case Qt::Key_ScrollLock:
        return KEY_SCROLLLOCK;

    case Qt::Key_Home:
        return KEY_HOME;
    case Qt::Key_Up:
        return KEY_UP;
    case Qt::Key_PageUp:
        return KEY_PAGEUP;
    case Qt::Key_Left:
        return KEY_LEFT;
    case Qt::Key_Right:
        return KEY_RIGHT;
    case Qt::Key_End:
        return KEY_END;
    case Qt::Key_Down:
        return KEY_DOWN;
    case Qt::Key_PageDown:
        return KEY_PAGEDOWN;
    case Qt::Key_Insert:
        return KEY_INSERT;
    case Qt::Key_Delete:
        return KEY_DELETE;

    case Qt::Key_Pause:
        return KEY_PAUSE;

    case Qt::Key_Help:
        return KEY_HELP;

    case Qt::Key_Print:
        return KEY_PRINT;

    case Qt::Key_F1:
        return KEY_F1;
    case Qt::Key_F2:
        return KEY_F2;
    case Qt::Key_F3:
        return KEY_F3;
    case Qt::Key_F4:
        return KEY_F4;
    case Qt::Key_F5:
        return KEY_F5;
    case Qt::Key_F6:
        return KEY_F6;
    case Qt::Key_F7:
        return KEY_F7;
    case Qt::Key_F8:
        return KEY_F8;
    case Qt::Key_F9:
        return KEY_F9;
    case Qt::Key_F10:
        return KEY_F10;
    case Qt::Key_F11:
        return KEY_F11;
    case Qt::Key_F12:
        return KEY_F12;
    case Qt::Key_F13:
        return KEY_F13;
    case Qt::Key_F14:
        return KEY_F14;
    case Qt::Key_F15:
        return KEY_F15;
    case Qt::Key_F16:
        return KEY_F16;

    default:
        Q_ASSERT(false);
        return INVALID_KEYCODE;
    }
}

//
// ------------------------------ AutoTypeExecutorWayland ------------------------------
//

AutoTypeExecutorWayland::AutoTypeExecutorWayland(AutoTypePlatformWayland* platform)
    : m_platform(platform)
{
}

void AutoTypeExecutorWayland::execChar(AutoTypeChar* action)
{
    m_platform->sendChar(action->character, true);
    m_platform->sendChar(action->character, false);
    usleep(25 * 1000);
}

void AutoTypeExecutorWayland::execKey(AutoTypeKey* action)
{
    m_platform->sendKey(action->key, true);
    m_platform->sendKey(action->key, false);
    usleep(25 * 1000);
}
