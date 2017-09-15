set(screenlock_mac_mm_SOURCES
    ScreenLockPlatformMac.mm
)

add_library(keepassx-screenlock-cocoa MODULE ${screenlock_mac_mm_SOURCES})
set_target_properties(keepassx-screenlock-cocoa PROPERTIES LINK_FLAGS "-framework Foundation -framework AppKit")
target_link_libraries(keepassx-screenlock-cocoa ${PROGNAME} Qt5::Core Qt5::Widgets)
add_custom_command(TARGET keepassx-screenlock-cocoa
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libkeepassx-screenlock-cocoa.so ${PLUGIN_INSTALL_DIR}
                   COMMAND ${QT_BINARY_DIR}/macdeployqt ${PROGNAME}.app -executable=${PLUGIN_INSTALL_DIR}/libkeepassx-screenlock-cocoa.so -no-plugins
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src
                   COMMENT "Deploying screenlock plugin")
