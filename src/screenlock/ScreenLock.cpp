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

#include "core/FilePath.h"

ScreenLock::ScreenLock(QWidget* parent)
    : QObject(parent)
    , m_pluginLoader(new QPluginLoader(this))
    , m_plugin(nullptr)
{
    // prevent crash when the plugin has unresolved symbols
    m_pluginLoader->setLoadHints(QLibrary::ResolveAllSymbolsHint);

    QString pluginName = "keepassx-screenlock-" + QApplication::platformName();
    QString pluginPath = filePath()->pluginPath(pluginName);

    if (!pluginPath.isEmpty()) {
        loadPlugin(pluginPath);
    }
}

void ScreenLock::loadPlugin(const QString& pluginPath)
{
    m_pluginLoader->setFileName(pluginPath);

    QObject* pluginInstance = m_pluginLoader->instance();
    if (pluginInstance) {
        m_plugin = qobject_cast<ScreenLockPlatformInterface*>(pluginInstance);
        if (m_plugin) {
            m_plugin->init(this);
        }
    }

    if (!m_plugin) {
        qWarning("Unable to load screenlock plugin:\n%s", qPrintable(m_pluginLoader->errorString()));
    }
}
