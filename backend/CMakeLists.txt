include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(
    Aptbrowserbackend_SRCS
    modules/Aptbrowser/backend.cpp
    modules/Aptbrowser/depends.cpp
)

add_library(Aptbrowserbackend MODULE
    ${Aptbrowserbackend_SRCS}
)

set_target_properties(Aptbrowserbackend PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY Aptbrowser)

qt5_use_modules(Aptbrowserbackend Gui Qml Quick)

# Copy qmldir file to build dir for running in QtCreator
add_custom_target(Aptbrowserbackend-qmldir ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/modules/Aptbrowser/qmldir ${CMAKE_CURRENT_BINARY_DIR}/Aptbrowser
    DEPENDS ${QMLFILES}
)

# Install plugin file
install(TARGETS Aptbrowserbackend DESTINATION ${QT_IMPORTS_DIR}/Aptbrowser/)
install(FILES   modules/Aptbrowser/qmldir DESTINATION ${QT_IMPORTS_DIR}/Aptbrowser/)

