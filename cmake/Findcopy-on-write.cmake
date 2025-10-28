if(TARGET stlab::copy-on-write)
  set(copy-on-write_FOUND TRUE)
  return()
endif()

# If not found and CPM is enabled, fetch it
if(NOT STLAB_DISABLE_CPM)
  if(NOT COMMAND CPMAddPackage)
    include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake)
  endif()
  
  # https://github.com/stlab/copy-on-write
  CPMAddPackage("gh:stlab/copy-on-write@1.0.6")
  
  if(copy-on-write_ADDED)
    # Should be done in the copy-on-write repo
    include(GNUInstallDirs)
    install(TARGETS stlab-copy-on-write
      EXPORT stlab-targets
      FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    set(copy-on-write_FOUND TRUE)
  endif()
else()
  message(FATAL_ERROR "copy-on-write not found and STLAB_DISABLE_CPM is ON")
endif()
