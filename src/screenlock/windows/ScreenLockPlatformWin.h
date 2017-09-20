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

#ifndef SCREENLOCK_PLATFORM_WIN_H
#define SCREENLOCK_PLATFORM_WIN_H

#include <windows.h>
#include <QObject>

#include "screenlock/ScreenLockPlatformInterface.h"

class ScreenLockPlatformWin : public QObject, public ScreenLockPlatformInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.keepassx.ScreenLockPlatformWin")
    Q_INTERFACES(ScreenLockPlatformInterface)

public:
    ScreenLockPlatformWin();
    ~ScreenLockPlatformWin();

    void init(QWidget* mainWindow) override;
    int platformEventFilter(void* message) override;

Q_SIGNALS:
    void locked();

private:
    HWND m_hwnd;
    HPOWERNOTIFY m_powerNotify;

    int dispatchPowerNotification(const MSG* msg);
    int dispatchSessionNotification(const MSG* msg);
};

#endif  // SCREENLOCK_PLATFORM_WIN_H
