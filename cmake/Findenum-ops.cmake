if(TARGET stlab::enum-ops)
  set(enum-ops_FOUND TRUE)
  return()
endif()

# If not found and CPM is enabled, fetch it
if(NOT STLAB_DISABLE_CPM)
  if(NOT COMMAND CPMAddPackage)
    include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake)
  endif()
  
  # https://github.com/stlab/enum-ops
  CPMAddPackage("gh:stlab/enum-ops@1.1.0")
  
  if(enum-ops_ADDED)
    # Should be done in the enum-ops repo
    include(GNUInstallDirs)
    install(TARGETS stlab-enum-ops
      EXPORT stlab-targets
      FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    set(enum-ops_FOUND TRUE)
  endif()
else()
  message(FATAL_ERROR "enum-ops not found and STLAB_DISABLE_CPM is ON")
endif()
