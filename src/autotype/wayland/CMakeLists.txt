set(autotype_wayland_SOURCES
    AutoTypeWayland.cpp
    UInput.cpp
    XKBMap.cpp
    VirtualKeyboard.cpp
)

add_library(keepassx-autotype-wayland MODULE ${autotype_wayland_SOURCES})
target_link_libraries(keepassx-autotype-wayland Qt5::Core Qt5::Widgets ${XKBCOMMON_LIBRARIES})
install(TARGETS keepassx-autotype-wayland
        BUNDLE DESTINATION . COMPONENT Runtime
        LIBRARY DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT Runtime)
