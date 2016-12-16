import QtQuick 2.0
import Ubuntu.Components 1.2
import "components"
import com.canonical.Oxide 1.0

MainView {
    id: main_view
    property string policy: ""
    property string cli: ""

    // for cli functional testing purposes (autopilot-qt5)
    objectName: "mainView"

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "com.ubuntu.developer.knitzsche.aptbrowser"

    //automaticOrientation: true

    width: units.gu(160)
    height: units.gu(100)

    Tabs {
        id: tabs 
        Tab {
            title: "C++ Threads"
            page: Page {
                id: threadsPage
                title: "C++ threads"
                objectName: "threadsTab"
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                ThreadController {}
            }
        }
        Tab {
            title: "cli"
            page: Page {
                id: cliPage
                title: "Cli"
                Cli {
                }
            }
        }
        Tab {
            title: "Apt"
            page: Page {
                id: aptPage
                title: "Apt Cache"
                objectName: "aptTab"
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                Apt {}
            }
        }
        Tab {
            title: "Web"
            page: Page {
                id: webviewPage
                title: "Webview"
                WebView {
                    id: webview
                    width: parent.width
                    height: parent.height
                    Component.onCompleted: {
                        url = "http://kylenitzsche.com"
                    }
                }
            }
        }

    }
    function getPolicy(pkg) {
        details.apt_policy(pkg);
    }
}

