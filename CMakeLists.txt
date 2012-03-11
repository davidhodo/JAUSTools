#############################################################
#   Top level CMakeLists.txt file for the JAUSTools Project
#############################################################


CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# Use clang if available
IF(EXISTS /usr/bin/clang++)
  set(CMAKE_CXX_COMPILER /usr/bin/clang++)
ENDIF(EXISTS /usr/bin/clang++)

PROJECT(JAUSTools)

###############################################################################
# Tell CMake where to look for find moduleXYZ files
#SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" )

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


##########################################################
# Decide what to build
##########################################################

############# gCommon ################
SET( BUILD_PRIMITIVEJOYSTICK ON CACHE BOOL "Build Primitive Driver Joystick app")
IF(BUILD_PRIMITIVEJOYSTICK)
    SET(BUILD_DIRS ${BUILD_DIRS} PrimitiveDriverJoystick)
ENDIF(BUILD_PRIMITIVEJOYSTICK)

##########################################################
# Set up include directories that are needed by all proj.
#find_package(Boost COMPONENTS system filesystem thread REQUIRED)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

SET(JAUSLIBS ${JAUSLIBS} json openjaus-core openjaus)

SET( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE PATH
     "Output directory for the JAUSTools libraries" )
SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH
     "Output directory for the JAUSTools executables" )

###############################################################################
# be helpful and let all projects look for libraries here
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH} ${BOOST_LIB_DIR})

# GO FORTH!
SUBDIRS(${BUILD_DIRS})
