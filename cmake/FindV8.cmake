# Copyright 2019-2022, Collabora, Ltd.
#
# SPDX-License-Identifier: BSL-1.0
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
# Original Author:
# 2019-2022 Ryan Pavlik <ryan.pavlik@collabora.com>

#.rst:
# FindV8
# ---------------
#
# Find the V8 JavaScript engine
#
# Targets
# ^^^^^^^
#
# If successful, the following import target is created.
#
# ``V8::v8``
#
# Cache variables
# ^^^^^^^^^^^^^^^
#
# The following cache variable may also be set to assist/control the operation of this module:
#
# ``V8_ROOT_DIR``
#  The root to search for V8.

set(V8_ROOT_DIR
    "${V8_ROOT_DIR}"
    CACHE PATH "Root to search for V8")

include(FindPackageHandleStandardArgs)

macro(SEE VAR)
    message(STATUS "${VAR} ${${VAR}}")
endmacro()

#message(STATUS "Looking for ${V8_PACKAGE_NAME} in ${V8_PACKAGE_DIRS}")
message(STATUS "V8_ROOT_DIR ${V8_ROOT_DIR}")
SEE(V8_ROOT_DIR)
# Check for CMake config first.
find_package(V8 QUIET CONFIG)
if(V8_FOUND AND TARGET v8)
    # Found config, let's prefer it.
    find_package_handle_standard_args(v8 CONFIG_MODE)
    set(V8_LIBRARY v8)

else()
    # Manually find
    find_path(
        V8_INCLUDE_DIR
        NAMES v8.h
        PATHS ${V8_ROOT_DIR}/v8
        PATH_SUFFIXES include
        REQUIRED)
    message(STATUS ${V8_ROOT_DIR}/$<IF:$<CONFIG:DEBUG>,cmake-build-debug,cmake-build-release>)
    find_library(
        V8_LIBRARY
        NAMES v8
        PATHS ${V8_ROOT_DIR}/cmake-build-release
        REQUIRED)

    find_package_handle_standard_args(V8 REQUIRED_VARS V8_INCLUDE_DIR V8_LIBRARY)
endif()

if(V8_FOUND)
    set(V8_INCLUDE_DIRS "${V8_INCLUDE_DIR}")
    set(V8_LIBRARIES "${V8_LIBRARY}")
    if(NOT TARGET V8::V8)
        if(TARGET "${V8_LIBRARY}")
            # Alias if we found the config file
            add_library(V8::V8 ALIAS v8)
        else()
			add_library(V8::V8 UNKNOWN IMPORTED)
			set_target_properties(
					V8::V8 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
					"${V8_INCLUDE_DIR}")
			set_target_properties(
					V8::V8 PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES "C"
					IMPORTED_LOCATION "${V8_LIBRARY}")


        endif()
    endif()
    mark_as_advanced(V8_INCLUDE_DIR V8_LIBRARY)
endif()

mark_as_advanced(V8_ROOT_DIR)
