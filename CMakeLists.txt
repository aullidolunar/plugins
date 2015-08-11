# ./CMakeLists.txt

# cmake ..
# cmake -G "MinGW Makefiles" ..
# use -DUSE_UNICODE=OFF to build with ANSI charset


## Tell not to build from source directory
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	message (FATAL_ERROR "It is best, not to build from toplevel directory. Create and go to \"build\" directory and run from there")
endif ()

## Set package details:
# package name
set (_PN "Dialogs")
# package version: mayor.minor.macro.micro
set (_PV "3.0.2.1")
# package description
set (_PD "Dialogs NSIS plugin")
# me
set (AUTHOR "Joel Almeida")
# my email
set (EMAIL "aullidolunar@gmail.com")
# packer name
set (PACKER_PACKAGE_FILE_NAME "${_PN}-${_PV}-src.7z")

## It's suppose to override stuff
set (CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_SOURCE_DIR}/data/c_flag_overrides.cmake)

## cmake stuff
cmake_minimum_required (VERSION 3.0)
project (${_PN} VERSION ${_PV} LANGUAGES C)

## welcome message
message (STATUS "Starting building process for ${PROJECT_NAME} version ${PROJECT_VERSION}")

# We need to detect the right compiler version and flags:
# MSVC users: you need at least msvc 2005
# MinGW/gcc: you need at least version 4.7
if(MSVC)
	if (CMAKE_C_COMPILER_VERSION VERSION_LESS "14.0")
		message(FATAL_ERROR "Compiler ${CMAKE_C_COMPILER} has to be msvc2005 or better.")
	endif ()
elseif (MINGW)
	if (CMAKE_C_COMPILER_VERSION VERSION_LESS "4.7")
		message(FATAL_ERROR "Compiler ${CMAKE_C_COMPILER} has to be 4.7 or better.")
	endif ()
endif ()

# Added in 3.0.2.0
# Give the option to build in UNICODE or ANSI. Really? You need ANSI in modern days?
option (USE_UNICODE "Build plugin in UNICODE charset" ON)
if (USE_UNICODE)
	## Let's be unicode
	add_definitions (-DUNICODE -D_UNICODE)
	set (NSIS_LIB "pluginapi-x86-unicode.lib")
	set (PLUGIN_FILE_NAME "${PROJECT_NAME}W")
	set (PACKAGE_CHARSET "UNICODE")
else()
	set (NSIS_LIB "pluginapi-x86-ansi.lib")
	set (PLUGIN_FILE_NAME "${PROJECT_NAME}A")
	set (PACKAGE_CHARSET "ANSI")
endif ()
message (STATUS "Charset: ${PACKAGE_CHARSET}")

## Build release mode
set (CMAKE_BUILD_TYPE "Release")

## For the sanity of the project, create the config.h
if (EXISTS "${PROJECT_SOURCE_DIR}/src/config.h.in")
	configure_file ("${PROJECT_SOURCE_DIR}/src/config.h.in" "${PROJECT_BINARY_DIR}/config.h")
	add_definitions (-DHAVE_CONFIG_H)
endif ()

# archive the source code
find_program (7Z 7za NAMES 7z)
if (7Z)
	message (STATUS "Program 7z: ${7Z} (targets dist is created)")
	add_custom_target (
		dist
		COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${_PLN}"
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		COMMAND ${7Z} a -t7z ${PROJECT_BINARY_DIR}/${PACKER_PACKAGE_FILE_NAME} data src bin examples CMakeLists.txt README LICENSE
		COMMENT "${PACKER_PACKAGE_FILE_NAME} created"
	)
endif ()

# Go deep
add_subdirectory (src)
