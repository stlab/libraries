# CMake generated Testfile for 
# Source directory: D:/misc/GitHub/libraries/test
# Build directory: D:/misc/GitHub/libraries/build_debug/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(stlab.test.future "D:/misc/GitHub/libraries/build_debug/bin/stlab.test.future.test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(stlab.test.future "D:/misc/GitHub/libraries/build_debug/bin/stlab.test.future.test.exe")
else()
  add_test(stlab.test.future NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(stlab.test.serial_queue "D:/misc/GitHub/libraries/build_debug/bin/stlab.test.serial_queue.test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(stlab.test.serial_queue "D:/misc/GitHub/libraries/build_debug/bin/stlab.test.serial_queue.test.exe")
else()
  add_test(stlab.test.serial_queue NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(stlab.test.cow "D:/misc/GitHub/libraries/build_debug/bin/stlab.test.cow.test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(stlab.test.cow "D:/misc/GitHub/libraries/build_debug/bin/stlab.test.cow.test.exe")
else()
  add_test(stlab.test.cow NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(stlab.test.task "D:/misc/GitHub/libraries/build_debug/bin/stlab.test.task.test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(stlab.test.task "D:/misc/GitHub/libraries/build_debug/bin/stlab.test.task.test.exe")
else()
  add_test(stlab.test.task NOT_AVAILABLE)
endif()
