file(GLOB UI_QML_JS_FILES *.qml *.js)

# Make the files visible in the qtcreator tree
add_custom_target(aptbrowser_ui_QMlFiles ALL SOURCES ${UI_QML_JS_FILES})

MESSAGE("ui qml and js files:" ${UI_QML_JS_FILES})

install(FILES ${UI_QML_JS_FILES} DESTINATION ${APTBROWSER_DIR}/ui)

