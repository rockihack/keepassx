if(UNIX AND NOT APPLE)
  find_package(XKBCommon)

  if(XKBCOMMON_FOUND)
    include_directories(SYSTEM ${XKBCOMMON_INCLUDE_DIR})
    add_subdirectory(wayland)
  endif()
endif()

if(WITH_TESTS)
  add_subdirectory(test)
endif()
