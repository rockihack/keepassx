/*
 *  Copyright (C) 2017 Lennart Glauer <mail@lennart-glauer.de>
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

#include "ScreenLockPlatformWin.h"

#include <wtsapi32.h>

ScreenLockPlatformWin::ScreenLockPlatformWin()
    : m_hwnd(nullptr)
    , m_powerNotify(nullptr)
{
}

ScreenLockPlatformWin::~ScreenLockPlatformWin()
{
    if (m_powerNotify != nullptr) {
        ::UnregisterPowerSettingNotification(m_powerNotify);
    }
    if (m_hwnd != nullptr) {
        ::WTSUnRegisterSessionNotification(m_hwnd);
    }
}

void ScreenLockPlatformWin::init(QWidget* mainWindow)
{
    HWND hwnd = reinterpret_cast<HWND>(mainWindow->winId());
    if (!::IsWindow(hwnd)) {
        qWarning("Invalid hwnd");
        return;
    }
    m_hwnd = hwnd;

    if (!::WTSRegisterSessionNotification(
            m_hwnd,
            NOTIFY_FOR_THIS_SESSION)) {
        qWarning("WTSRegisterSessionNotification failed");
    }

    m_powerNotify = ::RegisterPowerSettingNotification(
            m_hwnd,
            &GUID_LIDSWITCH_STATE_CHANGE,
            DEVICE_NOTIFY_WINDOW_HANDLE);
    if (m_powerNotify == nullptr) {
        qWarning("RegisterPowerSettingNotification failed");
    }
}

int ScreenLockPlatformWin::platformEventFilter(void* message)
{
    MSG* msg = static_cast<MSG*>(message);

    switch (msg->message) {
    case WM_POWERBROADCAST:
        return dispatchPowerNotification(msg);
    case WM_WTSSESSION_CHANGE:
        return dispatchSessionNotification(msg);
    default:
        return -1;
    }
}

int ScreenLockPlatformWin::dispatchPowerNotification(const MSG* msg)
{
    Q_ASSERT(msg->message == WM_POWERBROADCAST);

    switch (msg->wParam) {
    case PBT_APMSUSPEND:  // System is suspending operation
        Q_EMIT locked();
        return 1;
    case PBT_POWERSETTINGCHANGE:  // A power setting change event has been received
    {
        const POWERBROADCAST_SETTING* setting = reinterpret_cast<const POWERBROADCAST_SETTING*>(msg->lParam);
        if (setting != nullptr && setting->DataLength == sizeof(DWORD)
                    && setting->PowerSetting == GUID_LIDSWITCH_STATE_CHANGE) {
            const DWORD lidState = *reinterpret_cast<const DWORD*>(&setting->Data);
            if (lidState == 0) {  // Lid was closed
                Q_EMIT locked();
                return 1;
            }
        }
        return -1;
    }
    default:
        return -1;
    }
}

int ScreenLockPlatformWin::dispatchSessionNotification(const MSG* msg)
{
    Q_ASSERT(msg->message == WM_WTSSESSION_CHANGE);

    switch (msg->wParam) {
    case WTS_CONSOLE_DISCONNECT:  // The session identified by lParam was disconnected from the console terminal or RemoteFX session
    case WTS_REMOTE_DISCONNECT:  // The session identified by lParam was disconnected from the remote terminal
    case WTS_SESSION_LOGOFF:  // A user has logged off the session identified by lParam
    case WTS_SESSION_LOCK:  // The session identified by lParam has been locked
        Q_EMIT locked();
        return 1;
    default:
        return -1;
    }
}
