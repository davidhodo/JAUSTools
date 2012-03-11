PROJECT(PrimitiveDriverJoystick)

INCLUDE_DIRECTORIES(include)

#what files are needed?
SET(JOYSTICK_SRCS src/main.cpp include/PrimitiveDriverJoystick.h src/PrimitiveDriverJoystick.cpp src/joystick.cpp include/joystick.h 
   ${RESOURCES})

# Add executable called gUSARSim
ADD_EXECUTABLE(PrimitiveDriverJoystick ${JOYSTICK_SRCS})

# add mobility library to link list
SET(JAUSLIBS ${JAUSLIBS} openjaus-mobility)

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
