set (VERSION "0.2")

project(scratch-qt C CXX)
cmake_minimum_required(VERSION 2.8.10)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -g")
#ADD_DEFINITIONS( -DQT_NO_DEBUG_OUTPUT )

# Do not remove this line, its required for the correct functionality of the Ubuntu-SDK
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Quick)
# find_package(ubuntu-sdk-libs)

# Automatically create moc files
set(CMAKE_AUTOMOC ON)

# Components PATH
execute_process(
    COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
    OUTPUT_VARIABLE ARCH_TRIPLET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(QT_IMPORTS_DIR "lib/${ARCH_TRIPLET}")

set(APP_NAME          scratch-qt)
set(QML_DIR "qml")
set(MAIN_QML          "main.qml")
set(ICON              "graphics/icon.png")

# Set install paths
set(CMAKE_INSTALL_PREFIX /)
set(DATA_DIR /)
set(DESKTOP_DIR ${DATA_DIR})
set(DESKTOP_FILE_NAME "aptbrowser.desktop")

set(EXEC "qmlscene $@ ${QML_DIR}/${MAIN_QML}")

install(DIRECTORY "app/graphics" DESTINATION ${DATA_DIR})
file(MAKE_DIRECTORY ubuntu-app-platform)
install(DIRECTORY "ubuntu-app-platform" DESTINATION ${DATA_DIR})

add_subdirectory(app)
add_subdirectory(backend)

add_custom_target("run" /usr/bin/qmlscene -I ${CMAKE_BINARY_DIR}/backend  ${CMAKE_SOURCE_DIR}/app/main.qml
                    WORKING_DIRECTORY ./app)
