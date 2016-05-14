#ifndef KEEPASSX_AUTOTYPEWINDOWS_H
#define KEEPASSX_AUTOTYPEWINDOWS_H

#include <QtPlugin>
#include <Windows.h>

#include "autotype/AutoTypePlatformPlugin.h"
#include "autotype/AutoTypeAction.h"

class AutoTypePlatformWin : public QObject, public AutoTypePlatformInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.keepassx.AutoTypePlatformWindows")
    Q_INTERFACES(AutoTypePlatformInterface)

public:
    AutoTypePlatformWin();
    bool isAvailable() override;
    QStringList windowTitles() override;
    WId activeWindow() override;
    QString activeWindowTitle() override;
    bool registerGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers) override;
    void unregisterGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers) override;
    int platformEventFilter(void* event) override;
    int initialTimeout() override;
    bool raiseWindow(WId window) override;
    AutoTypeExecutor* createExecutor() override;

    void sendChar(const QChar& ch);
    void sendKey(Qt::Key key);

Q_SIGNALS:
    void globalShortcutTriggered();

private:
    DWORD m_threadId;
    Qt::Key m_currentGlobalKey;
    Qt::KeyboardModifiers m_currentGlobalModifiers;

    static DWORD qtToNativeKeyCode(Qt::Key key);
    static DWORD qtToNativeModifiers(Qt::KeyboardModifiers modifiers);
    static BOOL isAltTabWindow(HWND hwnd);
    static BOOL CALLBACK windowTitleEnumProc(_In_ HWND hwnd, _In_ LPARAM lParam);
    static DWORD WINAPI hotkeyThreadProc(_In_ LPVOID lpParameter);
};

class AutoTypeExecutorWin : public AutoTypeExecutor
{
public:
    explicit AutoTypeExecutorWin(AutoTypePlatformWin* platform);

    void execChar(AutoTypeChar* action) override;
    void execKey(AutoTypeKey* action) override;

private:
    AutoTypePlatformWin* const m_platform;
};

#endif // KEEPASSX_AUTOTYPEWINDOWS_H
