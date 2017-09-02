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

#ifndef SCREENLOCK_BACKEND_WIN_H
#define SCREENLOCK_BACKEND_WIN_H

#include <windows.h>
#include <QAbstractNativeEventFilter>
#include <QWidget>

#include "ScreenLockBackend.h"

class ScreenLock;

class ScreenLockBackendWin : public ScreenLockBackend, public QAbstractNativeEventFilter
{
    ScreenLock* const m_screenlock;
    HWND m_hwnd;
    HPOWERNOTIFY m_powerNotify;

public:
    ScreenLockBackendWin(ScreenLock* const screenlock, WId window);
    ~ScreenLockBackendWin();

    bool nativeEventFilter(const QByteArray &eventType, void* message, long* result) override;

private:
    bool dispatchPowerNotification(const MSG* msg);
    bool dispatchSessionNotification(const MSG* msg);
};

#endif  // SCREENLOCK_BACKEND_WIN_H
