file(GLOB COMPONENTS_QML_JS_FILES *.qml *.js)

# Make the files visible in the qtcreator tree
add_custom_target(aptbrowser_components_QMlFiles ALL SOURCES ${COMPONENTS_QML_JS_FILES})

install(FILES ${COMPONENTS_QML_JS_FILES} DESTINATION ${APTBROWSER_DIR}/components)

