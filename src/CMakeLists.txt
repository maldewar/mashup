FILE (GLOB_RECURSE project_SRCS main.cc *.cpp *.cxx */*.cc *.C *.c *.h *.hpp)
SET (project_LIBS ${Boost_LIBRARIES} ${Log4Cxx_LIBRARIES} ${Glib_LIBRARIES} ${Glib_GOBJECT_LIBRARIES} ${GSTREAMER_LIBRARIES})
SET (project_BIN ${PROJECT_NAME})

##############
#  MAIN APP  #
##############
ADD_EXECUTABLE(${project_BIN} ${project_SRCS})

TARGET_LINK_LIBRARIES(${project_BIN} ${project_LIBS})
SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS ${project_BIN} DESTINATION bin)

###############################
#  MASHSCRIPT PARSER COMMAND  #
###############################
FILE (GLOB_RECURSE mashscript_parser_SRCS mashscript-parser.cc *.cpp *.cxx */*.cc *.C *.c *.h *.hpp)
SET (mashscript_parser_LIBS ${Boost_LIBRARIES} ${Log4Cxx_LIBRARIES} ${Glib_LIBRARIES} ${Glib_GOBJECT_LIBRARIES} ${GSTREAMER_LIBRARIES})
SET (mashscript_parser_BIN "mashscript-parser")
ADD_EXECUTABLE(${mashscript_parser_BIN} ${mashscript_parser_SRCS})

TARGET_LINK_LIBRARIES(${mashscript_parser_BIN} ${mashscript_parser_LIBS})
SET_TARGET_PROPERTIES(${mashscript_parser_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${mashscript_parser_BIN} CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS ${mashscript_parser_BIN} DESTINATION bin)

######################
#  MASHUP PIPELINER  #
######################
FILE (GLOB_RECURSE mashup_pipeliner_SRCS mashup-pipeliner.cc *.cpp *.cxx */*.cc *.C *.c *.h *.hpp)
MESSAGE(ALERT "GOBJECT LIBRARIES: ${GLIB_GOBJECT_LIBRARIES}")
SET (mashup_pipeliner_LIBS ${Boost_LIBRARIES} ${Log4Cxx_LIBRARIES} ${Glib_LIBRARIES} ${Glib_GOBJECT_LIBRARIES} ${GSTREAMER_LIBRARIES})
SET (mashup_pipeliner_BIN "mashup-pipeliner")
ADD_EXECUTABLE(${mashup_pipeliner_BIN} ${mashup_pipeliner_SRCS})

TARGET_LINK_LIBRARIES(${mashup_pipeliner_BIN} ${mashup_pipeliner_LIBS})
SET_TARGET_PROPERTIES(${mashup_pipeliner_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${mashup_pipeliner_BIN} CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS ${mashup_pipeliner_BIN} DESTINATION bin)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
