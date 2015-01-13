# src/CMakeLists.txt

message (STATUS "+- entering SRC directory -+")

# sources
set (_SOURCES
	plugin.c
)

# target creation
include_directories (${PROJECT_BINARY_DIR})
add_library (${PROJECT_NAME} SHARED ${_SOURCES} ${LINKER_FLAGS})
set_target_properties (${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# install destination
install (TARGETS ${PROJECT_NAME} DESTINATION "${GEANY_LIBDIR}/geany")
