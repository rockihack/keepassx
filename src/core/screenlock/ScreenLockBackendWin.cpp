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

#include "ScreenLockBackendWin.h"

#include <wtsapi32.h>
#include <QApplication>

#include "ScreenLock.h"

ScreenLockBackendWin::ScreenLockBackendWin(ScreenLock* const screenlock, WId window)
    : QAbstractNativeEventFilter()
    , m_screenlock(screenlock)
    , m_hwnd(reinterpret_cast<HWND>(window))
    , m_powerNotify(nullptr)
{
    QCoreApplication::instance()->installNativeEventFilter(this);

    if (!WTSRegisterSessionNotification(
            m_hwnd,
            NOTIFY_FOR_THIS_SESSION)) {
        qWarning("WTSRegisterSessionNotification failed");
    }

    m_powerNotify = RegisterPowerSettingNotification(
            m_hwnd,
            &GUID_LIDSWITCH_STATE_CHANGE,
            DEVICE_NOTIFY_WINDOW_HANDLE);
    if (m_powerNotify == nullptr) {
        qWarning("RegisterPowerSettingNotification failed");
    }
}

ScreenLockBackendWin::~ScreenLockBackendWin()
{
    if (m_powerNotify != nullptr) {
        UnregisterPowerSettingNotification(m_powerNotify);
    }
    if (m_hwnd != nullptr) {
        WTSUnRegisterSessionNotification(m_hwnd);
    }

    QCoreApplication::instance()->removeNativeEventFilter(this);
}

bool ScreenLockBackendWin::nativeEventFilter(const QByteArray &eventType, void* message, long* result)
{
    Q_UNUSED(result);

    if (eventType == QByteArrayLiteral("windows_generic_MSG")
            || eventType == QByteArrayLiteral("windows_dispatcher_MSG")) {
        MSG* msg = static_cast<MSG*>(message);

        switch (msg->message) {
        case WM_POWERBROADCAST:
            return dispatchPowerNotification(msg);
        case WM_WTSSESSION_CHANGE:
            return dispatchSessionNotification(msg);
        default:
            return false;
        }
    }

    return false;
}

bool ScreenLockBackendWin::dispatchPowerNotification(const MSG* msg)
{
    switch (msg->wParam) {
    case PBT_APMSUSPEND:  // System is suspending operation
        Q_EMIT m_screenlock->locked();
        return true;
    case PBT_POWERSETTINGCHANGE:  // A power setting change event has been received
    {
        const POWERBROADCAST_SETTING* setting = reinterpret_cast<const POWERBROADCAST_SETTING*>(msg->lParam);
        if (setting != nullptr && setting->DataLength == sizeof(DWORD)
                    && setting->PowerSetting == GUID_LIDSWITCH_STATE_CHANGE) {
            const DWORD lidState = *reinterpret_cast<const DWORD*>(&setting->Data);
            if (lidState == 0) {  // Lid was closed
                Q_EMIT m_screenlock->locked();
                return true;
            }
        }
        return false;
    }
    default:
        return false;
    }
}

bool ScreenLockBackendWin::dispatchSessionNotification(const MSG* msg)
{
    switch (msg->wParam) {
    case WTS_CONSOLE_DISCONNECT:  // The session identified by lParam was disconnected from the console terminal or RemoteFX session
    case WTS_REMOTE_DISCONNECT:  // The session identified by lParam was disconnected from the remote terminal
    case WTS_SESSION_LOGOFF:  // A user has logged off the session identified by lParam
    case WTS_SESSION_LOCK:  // The session identified by lParam has been locked
        Q_EMIT m_screenlock->locked();
        return true;
    default:
        return false;
    }
}
