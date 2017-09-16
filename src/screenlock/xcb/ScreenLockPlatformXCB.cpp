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

#include "ScreenLockPlatformXCB.h"

#include <QDBusConnection>

ScreenLockPlatformXCB::ScreenLockPlatformXCB()
{
}

ScreenLockPlatformXCB::~ScreenLockPlatformXCB()
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    // Gnome
    sessionBus.disconnect(
        "org.gnome.SessionManager",
        "/org/gnome/SessionManager/Presence",
        "org.gnome.SessionManager.Presence",
        "StatusChanged",
        this,
        SLOT(gnomePresenceStatusChanged(uint)));
    sessionBus.disconnect(
        "org.gnome.ScreenSaver",
        "/org/gnome/ScreenSaver",
        "org.gnome.ScreenSaver",
        "ActiveChanged",
        this,
        SLOT(screensaverActiveChanged(bool)));

    // KDE
    sessionBus.disconnect(
        "org.freedesktop.ScreenSaver",
        "/org/freedesktop/ScreenSaver",
        "org.freedesktop.ScreenSaver",
        "ActiveChanged",
        this,
        SLOT(screensaverActiveChanged(bool)));
}

void ScreenLockPlatformXCB::init(QWidget* mainWindow)
{
    Q_UNUSED(mainWindow);

    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    // Gnome
    sessionBus.connect(
        "org.gnome.SessionManager",
        "/org/gnome/SessionManager/Presence",
        "org.gnome.SessionManager.Presence",
        "StatusChanged",
        this,
        SLOT(gnomePresenceStatusChanged(uint)));
    sessionBus.connect(
        "org.gnome.ScreenSaver",
        "/org/gnome/ScreenSaver",
        "org.gnome.ScreenSaver",
        "ActiveChanged",
        this,
        SLOT(screensaverActiveChanged(bool)));

    // KDE
    sessionBus.connect(
        "org.freedesktop.ScreenSaver",
        "/org/freedesktop/ScreenSaver",
        "org.freedesktop.ScreenSaver",
        "ActiveChanged",
        this,
        SLOT(screensaverActiveChanged(bool)));
}

int ScreenLockPlatformXCB::platformEventFilter(void* message)
{
    Q_UNUSED(message);
    Q_ASSERT(false);

    return -1;
}

void ScreenLockPlatformXCB::screensaverActiveChanged(bool active)
{
    if (active) {
        Q_EMIT locked();
    }
}

void ScreenLockPlatformXCB::gnomePresenceStatusChanged(uint status)
{
    if (status == 3) {  // Idle
        Q_EMIT locked();
    }
}
