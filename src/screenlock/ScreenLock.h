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

#ifndef SCREENLOCK_H
#define SCREENLOCK_H

#include <QObject>

class QAbstractNativeEventFilter;
class QPluginLoader;
class ScreenLockPlatformInterface;

class ScreenLock : public QObject
{
    Q_OBJECT

public:
    ScreenLock(QWidget* parent = nullptr);
    ~ScreenLock();

    int callEventFilter(void* message) const;

Q_SIGNALS:
    void locked();

private:
    QAbstractNativeEventFilter* const m_eventFilter;
    QPluginLoader* const m_pluginLoader;
    ScreenLockPlatformInterface* m_plugin;

    void loadPlugin(const QString& pluginPath, QWidget* parent);

    Q_DISABLE_COPY(ScreenLock)
};

#endif  // SCREENLOCK_H
