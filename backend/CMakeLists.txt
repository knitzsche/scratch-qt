include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(
    Scratchqtbackend_SRCS
    plugin/backend.cpp
    plugin/cli.cpp
    plugin/jobcontroller.cpp
    plugin/job.cpp
    plugin/jobs.cpp
)

add_library(Scratchqtbackend MODULE
    ${Scratchqtbackend_SRCS}
)

set_target_properties(Scratchqtbackend PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY Scratchqt)

qt5_use_modules(Scratchqtbackend Gui Qml Quick)

# Copy qmldir file to build dir for running in QtCreator
add_custom_target(Scratchqtbackend-qmldir ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/plugin/qmldir ${CMAKE_CURRENT_BINARY_DIR}/Scratchqt
    DEPENDS ${QMLFILES}
)

# Install plugin file
install(TARGETS Scratchqtbackend DESTINATION ${QT_IMPORTS_DIR}/Scratchqt/)
install(FILES   plugin/qmldir DESTINATION ${QT_IMPORTS_DIR}/Scratchqt/)

