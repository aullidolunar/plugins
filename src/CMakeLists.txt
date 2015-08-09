# src/CMakeLists.txt

## Let's be unicode
add_definitions (-DUNICODE -D_UNICODE)

## Be sure we have NSIS installed AND its installation directory in PATH
if (EXISTS "$ENV{NSIS}/Examples/Plugin/nsis/pluginapi-x86-unicode.lib")
	include_directories ("$ENV{NSIS}/Examples/Plugin")
	link_directories ("$ENV{NSIS}/Examples/Plugin/nsis")
	link_libraries ("pluginapi-x86-unicode.lib")
else ()
	message(FATAL_ERROR "Sorry, I couldn't find the nsis library for plugins.")
endif ()

# do win32 resource
# Generate proper resource file output
set (RESOURCE_FILE_IN "${PROJECT_SOURCE_DIR}/data/resource.rc.in")
if (EXISTS "${RESOURCE_FILE_IN}" AND WIN32)
	if (MSVC)
		find_program(RC_PATH rc)
		set (RESOURCE_OBJECT "${PROJECT_BINARY_DIR}/resource.res")
	else ()
		find_program(RC_PATH windres)
		set (RESOURCE_OBJECT "${PROJECT_BINARY_DIR}/resource.o")
	endif ()
	if (NOT RC_PATH)
		message (FATAL_ERROR "Resource compiler not found (required)")
	else ()
		set (RESOURCE_FILE_OUT "${PROJECT_BINARY_DIR}/resource.rc")
		configure_file ("${RESOURCE_FILE_IN}" "${RESOURCE_FILE_OUT}")
		if (MSVC)
			set (RC_CMD ${RC_PATH} /I ${PROJECT_SOURCE_DIR}/data ${RESOURCE_FILE_OUT})
		else ()
			set (RC_CMD ${RC_PATH} -I${PROJECT_SOURCE_DIR}/data ${RESOURCE_FILE_OUT} -o ${RESOURCE_OBJECT}) 
		endif ()
		add_custom_command (OUTPUT "${RESOURCE_OBJECT}" COMMAND ${RC_CMD} COMMENT "Compiling resource file")
		add_custom_target (resourcefile DEPENDS ${RESOURCE_OBJECT})
		set_source_files_properties (${RESOURCE_OBJECT} PROPERTIES EXTERNAL_OBJECT true GENERATED true)
		list (APPEND _SOURCES ${RESOURCE_OBJECT})
	endif ()
endif ()

# Sources
list (APPEND _SOURCES callbacks.c Dialogs.c)

## add directories to the search path
include_directories (${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/data)

## Library creation
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY )
add_library (${PROJECT_NAME} MODULE ${_SOURCES})
set_target_properties (${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
if (MSVC)
	set_target_properties (${PROJECT_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:\"LIBC.lib\" /OPT:NOWIN98 /MANIFEST:NO")
endif ()
