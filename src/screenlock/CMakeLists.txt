if(WIN32)
  add_subdirectory(windows)
elseif(APPLE)
  add_subdirectory(mac)
elseif(UNIX)
  find_package(Qt5DBus 5.2 REQUIRED)

  add_subdirectory(xcb)
endif()
