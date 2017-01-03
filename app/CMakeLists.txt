file(GLOB QML_JS_FILES *.qml *.js)

# Make the files visible on qtcreator
add_custom_target(aptbrowser_QMlFiles ALL SOURCES ${QML_JS_FILES})

# Substitute variables in the desktop file
#configure_file(${DESKTOP_FILE_NAME}.in ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME}.in)

install(FILES ${QML_JS_FILES} DESTINATION ${QML_DIR})

add_subdirectory(components)
add_subdirectory(ui)

