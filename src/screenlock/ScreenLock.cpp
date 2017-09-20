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

#include <QApplication>
#include <QAbstractNativeEventFilter>
#include <QPluginLoader>

#include "core/FilePath.h"
#include "screenlock/ScreenLockPlatformInterface.h"

#if defined(Q_OS_WIN)
class ScreenLockEventFilterWin : public QAbstractNativeEventFilter
{
    ScreenLock* m_screenlock;

public:
    ScreenLockEventFilterWin(ScreenLock* screenlock)
        : m_screenlock(screenlock)
    {
    };

    bool nativeEventFilter(const QByteArray &eventType, void* message, long* result) override
    {
        Q_UNUSED(result);

        if (eventType == QByteArrayLiteral("windows_generic_MSG")
                || eventType == QByteArrayLiteral("windows_dispatcher_MSG")) {
            int retCode = m_screenlock->callEventFilter(message);
            if (retCode == 1) {
                return true;
            }
        }

        return false;
    };
};
#endif

ScreenLock::ScreenLock(QWidget* mainWindow)
    : QObject(mainWindow)

#if defined(Q_OS_WIN)
    , m_eventFilter(new ScreenLockEventFilterWin(this))
#else
    , m_eventFilter(nullptr)
#endif

    , m_pluginLoader(new QPluginLoader(this))
    , m_plugin(nullptr)
{
    if (m_eventFilter) {
        QApplication::instance()->installNativeEventFilter(m_eventFilter);
    }

    // prevent crash when the plugin has unresolved symbols
    m_pluginLoader->setLoadHints(QLibrary::ResolveAllSymbolsHint);

    QString pluginName = "keepassx-screenlock-" + QApplication::platformName();
    QString pluginPath = filePath()->pluginPath(pluginName);

    if (!pluginPath.isEmpty()) {
        loadPlugin(pluginPath, mainWindow);
    }
}

ScreenLock::~ScreenLock()
{
    if (m_eventFilter) {
        QApplication::instance()->removeNativeEventFilter(m_eventFilter);
    }
}

void ScreenLock::loadPlugin(const QString& pluginPath, QWidget* mainWindow)
{
    m_pluginLoader->setFileName(pluginPath);

    QObject* pluginInstance = m_pluginLoader->instance();
    if (pluginInstance) {
        m_plugin = qobject_cast<ScreenLockPlatformInterface*>(pluginInstance);
        if (m_plugin) {
            m_plugin->init(mainWindow);
            connect(pluginInstance, SIGNAL(locked()), this, SIGNAL(locked()));
        }
    }

    if (!m_plugin) {
        qWarning("Unable to load screenlock plugin:\n%s", qPrintable(m_pluginLoader->errorString()));
    }
}

int ScreenLock::callEventFilter(void* message) const
{
    if (!m_plugin) {
        return -1;
    }

    return m_plugin->platformEventFilter(message);
}
