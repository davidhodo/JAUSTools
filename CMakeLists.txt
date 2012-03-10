## Project Setup
cmake_minimum_required(VERSION 2.4.6)

project(network_GPS)

## Configurations

# Use clang if available
IF(EXISTS /usr/bin/clang)
  set(CMAKE_CXX_COMPILER /usr/bin/clang++)
ENDIF(EXISTS /usr/bin/clang)


# Allow for building shared libs override
IF(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF)
ENDIF(NOT BUILD_SHARED_LIBS)

# Allow for build type override
IF(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
ENDIF(NOT CMAKE_BUILD_TYPE)

# set the default path for built executables to the "bin" directory
IF(NOT EXECUTABLE_OUTPUT_PATH)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)
# set the default path for built libraries to the "lib" directory
IF(NOT LIBRARY_OUTPUT_PATH)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
ENDIF(NOT LIBRARY_OUTPUT_PATH)

## Configure build system

# Add the include folder to the include path
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add default source files
set(GPS_SRCS src/MockGPS.cpp include/GPSNetworkInterface.h include/MockGPS.h)

# Add header files
set(GPS_HEADERS ${PROJECT_SOURCE_DIR}/include/GPSNetworkInterface.h ${PROJECT_SOURCE_DIR}/include/MockGPS.h)

## Build the library
#set(GPS_LINK_LIBS  serial)

# Compile the GPS Interface Library
add_library(atrv ${ATRV_SRCS})
target_link_libraries(atrv ${ATRV_LINK_LIBS})
## Build Examples

# If specified
IF(ATRV_BUILD_EXAMPLES)
  # Compile the Example program
  add_executable(atrv_example examples/atrv_example.cpp)
  # Link the Test program to the atrv library
  target_link_libraries(atrv_example atrv ${ATRV_LINK_LIBS})
ENDIF(ATRV_BUILD_EXAMPLES)

## Build Tests

# If specified
IF(ATRV_BUILD_TESTS)
  # Find Google Test
  enable_testing()
  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})

  # Compile the Test program
  add_executable(atrv_tests tests/atrv_tests.cpp)
  # Link the Test program to the atrv library
  target_link_libraries(atrv_tests atrv ${GTEST_BOTH_LIBRARIES})
  add_test(AllTestsIntest_atrv atrv_tests)
ENDIF(ATRV_BUILD_TESTS)

# Configure make install
IF(NOT CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX /usr/local)
ENDIF(NOT CMAKE_INSTALL_PREFIX)

## Install setup

# Install the library
IF(NOT ATRV_DONT_SETUP_INSTALL)
    INSTALL(
            TARGETS atrv
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
    )

    INSTALL(FILES ${ATRV_HEADERS} DESTINATION include/atrv)

    IF(NOT CMAKE_FIND_INSTALL_PATH)
        set(CMAKE_FIND_INSTALL_PATH ${CMAKE_ROOT})
    ENDIF(NOT CMAKE_FIND_INSTALL_PATH)

    INSTALL(FILES Findatrv.cmake DESTINATION
            ${CMAKE_FIND_INSTALL_PATH}/Modules/)

    ADD_CUSTOM_TARGET(uninstall @echo uninstall package)

    IF(UNIX)
      ADD_CUSTOM_COMMAND(
                         COMMENT "uninstall package"
                         COMMAND xargs ARGS rm < install_manifest.txt
                         TARGET  uninstall
      )
    ELSE(UNIX)
      ADD_CUSTOM_COMMAND(
                         COMMENT "uninstall only implemented in unix"
                         TARGET  uninstall
      )
    ENDIF(UNIX)
ENDIF(NOT ATRV_DONT_SETUP_INSTALL)
