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

#include "ScreenLock.h"

#if defined(Q_OS_WIN)
#   include "ScreenLockBackendWin.h"
#elif defined(Q_OS_OSX)
#   include "ScreenLockBackendMac.h"
#elif defined(Q_OS_LINUX)
#   include "ScreenLockBackendLinux.h"
#endif

ScreenLock::ScreenLock(QWidget* parent)
    : QObject(parent)
#if defined(Q_OS_WIN)
    , m_backend(new ScreenLockBackendWin(this, parent->winId()))
#elif defined(Q_OS_OSX)
    , m_backend(new ScreenLockBackendMac(this))
#elif defined(Q_OS_LINUX)
    , m_backend(new ScreenLockBackendLinux(this))
#endif
{
}
