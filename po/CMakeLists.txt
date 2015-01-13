# po/CMakeLists.txt

message (STATUS "+- entering PO directory -+")

find_program (XGETTEXT xgettext)
if (NOT XGETTEXT)
	message (WARNING "For developers: xgettext not found")
endif ()

find_program(INTLTOOLEXTRACT intltool-extract)
IF (NOT INTLTOOLEXTRACT)
	message (WARNING "For developers: intltool-extract not found")
endif ()

find_program(MSGFMT msgfmt)
IF (NOT MSGFMT)
	message (FATAL_ERROR "msgfmt not found (required)")
endif ()

find_program(MSGINIT msginit)
if (NOT MSGINIT)
	message (WARNING "For developers: msginit not found")
ENDIF ()

find_program(MSGMERGE msgmerge)
IF (NOT MSGMERGE)
	message (WARNING "For developers: msgmerge not found")
endif ()

add_custom_target(create-po
	COMMAND ${MSGINIT} --no-translator --no-wrap -i "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pot" -o "${PROJECT_SOURCE_DIR}/po/es.po" -l es_MX.utf8
)

add_custom_target(update-po
	COMMAND ${MSGMERGE} --update -N --no-wrap "${PROJECT_SOURCE_DIR}/po/es.po" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pot"
	COMMAND rm "${PROJECT_SOURCE_DIR}/po/es.po~"
)

set (CMD_BUILDER1)
set (CMD_BUILDER2)
if (EXISTS "${PROJECT_SOURCE_DIR}/data/${PROJECT_NAME}.builder")
	set (CMD_BUILDER1 ${INTLTOOLEXTRACT} --type="gettext/glade" "../../data/${PROJECT_NAME}.builder")
	set (CMD_BUILDER2 mv "../../data/${PROJECT_NAME}.builder.h" "${PROJECT_BINARY_DIR}")
else ()
	message ("-- No builder file found")
endif ()

set (CMD_DESKTOP)
if (EXISTS "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.desktop.in")
	set (CMD_DESKTOP ${INTLTOOLEXTRACT} --type="gettext/ini" "../${PROJECT_NAME}.desktop.in")
else ()
	message ("-- No desktop file found")
endif ()

add_custom_target(create-pot
	COMMAND ${CMD_BUILDER1}
	COMMAND ${CMD_BUILDER2}
	COMMAND ${CMD_DESKTOP}
	COMMAND ${XGETTEXT} -o "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pot" --no-wrap --copyright-holder=${EMAIL} --package-name=${PROJECT_NAME} --package-version=${PROJECT_VERSION} --msgid-bugs-address=${EMAIL} --keyword=_ --keyword=N_ --from-code=utf-8 ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_BINARY_DIR}/*.h
)

file (GLOB PO_FILES *.po)
list (LENGTH "${PO_FILES}" PO_FILES_LEN)
foreach(_poFile ${PO_FILES})
	get_filename_component(_lang ${_poFile} NAME_WE)
	if (NOT EXISTS "${PROJECT_BINARY_DIR}/locale/${_lang}/LC_MESSAGES/")
		add_custom_command (
			OUTPUT "${PROJECT_BINARY_DIR}/locale/${_lang}/LC_MESSAGES/"
			COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/locale/${_lang}/LC_MESSAGES/"
			COMMENT "Reading locale ${_lang}"
		)
		set (LOCALE_DIRS ${LOCALE_DIRS} "${PROJECT_BINARY_DIR}/locale/${_lang}/LC_MESSAGES/")
	endif ()
	add_custom_command(
		OUTPUT "${PROJECT_BINARY_DIR}/locale/${_lang}/LC_MESSAGES/${PROJECT_NAME}.mo"
		COMMAND ${MSGFMT} ${_poFile} -o "${PROJECT_BINARY_DIR}/locale/${_lang}/LC_MESSAGES/${PROJECT_NAME}.mo"
		COMMENT "Building ${_lang} translation"
	)
	set (MO_FILES ${MO_FILES} "${PROJECT_BINARY_DIR}/locale/${_lang}/LC_MESSAGES/${PROJECT_NAME}.mo")
	install (FILES "${PROJECT_BINARY_DIR}/locale/${_lang}/LC_MESSAGES/${PROJECT_NAME}.mo" DESTINATION "share/${PROJECT_NAME}/locale/${_lang}/LC_MESSAGES")
endforeach(_poFile ${PO_FILES})

add_custom_target (locales DEPENDS ${LOCALE_DIRS} ${MO_FILES})
add_dependencies (${PROJECT_NAME} locales)
