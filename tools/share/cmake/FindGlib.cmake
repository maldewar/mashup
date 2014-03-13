# - Try to find Glib and its components (gio, gobject etc)
# Once done, this will define
#
#  Glib_FOUND - system has Glib
#  Glib_INCLUDE_DIRS - the Glib include directories
#  Glib_LIBRARIES - link these to use Glib
#
# Optionally, the COMPONENTS keyword can be passed to find_package()
# and Glib components can be looked for.  Currently, the following
# components can be used, and they define the following variables if
# found:
#
#  gio:             Glib_GIO_LIBRARIES
#  gobject:         Glib_GOBJECT_LIBRARIES
#  gmodule:         Glib_GMODULE_LIBRARIES
#  gthread:         Glib_GTHREAD_LIBRARIES
#
# Note that the respective _INCLUDE_DIR variables are not set, since
# all headers are in the same directory as Glib_INCLUDE_DIRS.
#
# Copyright (C) 2012 Raphael Kubo da Costa <rakuco@webkit.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND ITS CONTRIBUTORS ``AS
# IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

find_package(PkgConfig)
pkg_check_modules(PC_GLIB QUIET glib-2.0)

find_library(Glib_LIBRARIES
    NAMES glib-2.0
    HINTS ${PC_Glib_LIBDIR}
          ${PC_Glib_LIBRARY_DIRS}
)

# Files in glib's main include path may include glibconfig.h, which,
# for some odd reason, is normally in $LIBDIR/glib-2.0/include.
get_filename_component(_Glib_LIBRARY_DIR ${Glib_LIBRARIES} PATH)
find_path(GLIBCONFIG_INCLUDE_DIR
    NAMES glibconfig.h
    HINTS ${PC_LIBDIR} ${PC_LIBRARY_DIRS} ${_Glib_LIBRARY_DIR}
    PATH_SUFFIXES glib-2.0/include
)

find_path(Glib_INCLUDE_DIR
    NAMES glib.h
    HINTS ${PC_Glib_INCLUDEDIR}
          ${PC_Glib_INCLUDE_DIRS}
    PATH_SUFFIXES glib-2.0
)

set(Glib_INCLUDE_DIRS ${Glib_INCLUDE_DIR} ${GLIBCONFIG_INCLUDE_DIR})

# Version detection
file(READ "${GLIBCONFIG_INCLUDE_DIR}/glibconfig.h" GLIBCONFIG_H_CONTENTS)
string(REGEX MATCH "#define Glib_MAJOR_VERSION ([0-9]+)" _dummy "${GLIBCONFIG_H_CONTENTS}")
set(Glib_VERSION_MAJOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define Glib_MINOR_VERSION ([0-9]+)" _dummy "${GLIBCONFIG_H_CONTENTS}")
set(Glib_VERSION_MINOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define Glib_MICRO_VERSION ([0-9]+)" _dummy "${GLIBCONFIG_H_CONTENTS}")
set(Glib_VERSION_MICRO "${CMAKE_MATCH_1}")
set(Glib_VERSION "${Glib_VERSION_MAJOR}.${Glib_VERSION_MINOR}.${Glib_VERSION_MICRO}")

# Additional Glib components.  We only look for libraries, as not all of them
# have corresponding headers and all headers are installed alongside the main
# glib ones.
foreach (_component ${Glib_FIND_COMPONENTS})
    if (${_component} STREQUAL "gio")
        find_library(Glib_GIO_LIBRARIES NAMES gio-2.0 HINTS ${_Glib_LIBRARY_DIR})
        set(ADDITIONAL_REQUIRED_VARS ${ADDITIONAL_REQUIRED_VARS} Glib_GIO_LIBRARIES)
    elseif (${_component} STREQUAL "gobject")
        find_library(Glib_GOBJECT_LIBRARIES NAMES gobject-2.0 HINTS ${_Glib_LIBRARY_DIR})
        set(ADDITIONAL_REQUIRED_VARS ${ADDITIONAL_REQUIRED_VARS} Glib_GOBJECT_LIBRARIES)
    elseif (${_component} STREQUAL "gmodule")
        find_library(Glib_GMODULE_LIBRARIES NAMES gmodule-2.0 HINTS ${_Glib_LIBRARY_DIR})
        set(ADDITIONAL_REQUIRED_VARS ${ADDITIONAL_REQUIRED_VARS} Glib_GMODULE_LIBRARIES)
    elseif (${_component} STREQUAL "gthread")
        find_library(Glib_GTHREAD_LIBRARIES NAMES gthread-2.0 HINTS ${_Glib_LIBRARY_DIR})
        set(ADDITIONAL_REQUIRED_VARS ${ADDITIONAL_REQUIRED_VARS} Glib_GTHREAD_LIBRARIES)
    elseif (${_component} STREQUAL "gio-unix")
        # gio-unix is compiled as part of the gio library, but the include paths
        # are separate from the shared glib ones. Since this is currently only used
        # by WebKitGTK+ we don't go to extraordinary measures beyond pkg-config.
        pkg_check_modules(GIO_UNIX QUIET gio-unix-2.0)
    endif ()
endforeach ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLIB REQUIRED_VARS Glib_INCLUDE_DIRS Glib_LIBRARIES ${ADDITIONAL_REQUIRED_VARS}
                                       VERSION_VAR   Glib_VERSION)

mark_as_advanced(
    GLIBCONFIG_INCLUDE_DIR
    Glib_GIO_LIBRARIES
    Glib_GIO_UNIX_LIBRARIES
    Glib_GMODULE_LIBRARIES
    Glib_GOBJECT_LIBRARIES
    Glib_GTHREAD_LIBRARIES
    Glib_INCLUDE_DIR
    Glib_INCLUDE_DIRS
    Glib_LIBRARIES
)
