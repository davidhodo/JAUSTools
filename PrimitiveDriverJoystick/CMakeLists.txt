PROJECT(PrimitiveDriverJoystick)

#what files are needed?
SET(JOYSTICK_SRCS main.cpp PrimitiveDriverJoystick.h PrimitiveDriverJoystick.cpp joystick.cpp joystick.h 
   ${RESOURCES})

# Add executable called gUSARSim
ADD_EXECUTABLE(PrimitiveDriverJoystick ${JOYSTICK_SRCS})

# link again multimedia library in windows
IF(WIN32)
        TARGET_LINK_LIBRARIES(PrimitiveDriverJoystick ${JAUSLIBS} "Winmm.lib")
ENDIF(WIN32)

IF(UNIX)
        TARGET_LINK_LIBRARIES(PrimitiveDriverJoystick ${JAUSLIBS})
ENDIF(UNIX)

INSTALL(TARGETS
PrimitiveDriverJoystick
RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
)
