# data/CMakeLists.txt

message (STATUS "+- entering DATA directory -+")

if (EXISTS "${PROJECT_SOURCE_DIR}/data/${PROJECT_NAME}.desktop.in.in")
	
	configure_file (
		"${PROJECT_SOURCE_DIR}/data/${PROJECT_NAME}.desktop.in.in"
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}.desktop.in"
	)
	set (DESKTOP_FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.desktop")
	
	find_program(INTLTOOLMERGE intltool-merge)
	
	if (NOT INTLTOOLMERGE)
		message (FATAL_ERROR "intltool-merge not found (required)")
	endif ()

	add_custom_command(
		OUTPUT "${DESKTOP_FILE}"
		COMMAND ${INTLTOOLMERGE} -d -u ../../po ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.desktop.in ${DESKTOP_FILE}
		COMMENT "Creating desktop file"
	)

	install (FILES "${DESKTOP_FILE}" DESTINATION "share/applications")

	add_custom_target (desktopfile DEPENDS ${DESKTOP_FILE})
	add_dependencies (${PROJECT_NAME} desktopfile)
endif ()

file (GLOB DATA_FILES *.*)
foreach (DATA_FILE ${DATA_FILES})
	get_filename_component (FILE_EXT ${DATA_FILE} EXT)
	if ((${FILE_EXT} MATCHES ".txt") OR (${FILE_EXT} MATCHES ".desktop.in.in"))
	else ()
		install (FILES "${DATA_FILE}" DESTINATION "share/${PROJECT_NAME}")
	endif()
endforeach ()

