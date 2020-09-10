set(screenlock_win_SOURCES
    ScreenLockPlatformWin.cpp
)

add_library(keepassx-screenlock-windows MODULE ${screenlock_win_SOURCES})
target_link_libraries(keepassx-screenlock-windows keepassx_core Qt5::Core Qt5::Widgets Wtsapi32.lib)
install(TARGETS keepassx-screenlock-windows
        BUNDLE DESTINATION . COMPONENT Runtime
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT Runtime)