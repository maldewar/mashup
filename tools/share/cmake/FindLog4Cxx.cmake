# - Try to find the liblog4cxx libraries
# Once done this will define
#
# Log4cxx_FOUND - system has liblog4cxx
# Log4Cxx_INCLUDE_DIRS - the liblog4cxx include directory
# Log4Cxx_LIBRARIES - liblog4cxx library

FIND_PATH(Log4Cxx_INCLUDE_DIRS logger.h PATHS /usr/include/log4cxx /usr/local/include/log4cxx )
FIND_LIBRARY(Log4Cxx_LIBRARIES NAMES log4cxx )

IF(Log4Cxx_INCLUDE_DIRS AND Log4Cxx_LIBRARIES)
  SET(Log4cxx_FOUND 1)
  #remove last /log4cxx string
  STRING(REGEX REPLACE "/log4cxx" "" Log4Cxx_INCLUDE_DIRS_SUP_LEVEL ${Log4Cxx_INCLUDE_DIRS})
  SET (Log4Cxx_INCLUDE_DIRS ${Log4Cxx_INCLUDE_DIRS_SUP_LEVEL} ${Log4Cxx_INCLUDE_DIRS} )
  if(NOT Log4cxx_FIND_QUIETLY)
   message(STATUS "Found log4cxx: ${Log4Cxx_LIBRARIES}")
  endif(NOT Log4cxx_FIND_QUIETLY)
ELSE(Log4Cxx_INCLUDE_DIRS AND Log4Cxx_LIBRARIES)
  SET(Log4cxx_FOUND 0 CACHE BOOL "Not found log4cxx library")
  message(STATUS "NOT Found log4cxx, disabling it")
ENDIF(Log4Cxx_INCLUDE_DIRS AND Log4Cxx_LIBRARIES)

MARK_AS_ADVANCED(Log4Cxx_INCLUDE_DIRS Log4Cxx_LIBRARIES)
