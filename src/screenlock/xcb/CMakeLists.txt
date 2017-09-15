set(screenlock_XCB_SOURCES
    ScreenLockPlatformXCB.cpp
)

add_library(keepassx-screenlock-xcb MODULE ${screenlock_XCB_SOURCES})
target_link_libraries(keepassx-screenlock-xcb Qt5::Core Qt5::Widgets Qt5::DBus)
install(TARGETS keepassx-screenlock-xcb
        BUNDLE DESTINATION . COMPONENT Runtime
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT Runtime)
