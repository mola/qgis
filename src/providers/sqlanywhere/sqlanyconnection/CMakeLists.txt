########################################################
# Files

SET (SACONN_SRCS 
    sqlanystatement.cpp
    sqlanyconnection.cpp
    sacapi/sacapidll.c
)

IF (WIN32)
  IF (MSVC)
    ADD_DEFINITIONS("-DSACONN_EXPORT=__declspec(dllexport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-USACONN_EXPORT \"-DSACONN_EXPORT=__declspec(dllexport)\"")
  ENDIF (MSVC)
ELSE (WIN32)
  ADD_DEFINITIONS(-DSACONN_EXPORT=)
ENDIF (WIN32)

########################################################
# Build

INCLUDE_DIRECTORIES(
     ../../../core 
)

ADD_LIBRARY (sqlanyconnection SHARED ${SACONN_SRCS})

SET_TARGET_PROPERTIES(sqlanyconnection PROPERTIES VERSION ${COMPLETE_VERSION} SOVERSION ${COMPLETE_VERSION})

TARGET_LINK_LIBRARIES(sqlanyconnection
  qgis_core
)


########################################################
# Install

IF (WIN32)
    INSTALL(TARGETS sqlanyconnection
      RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
      LIBRARY DESTINATION ${QGIS_PLUGIN_DIR}
      )
ELSE (WIN32)
    INSTALL(TARGETS sqlanyconnection
      RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
      LIBRARY DESTINATION ${QGIS_LIB_DIR}
      )
ENDIF (WIN32)

