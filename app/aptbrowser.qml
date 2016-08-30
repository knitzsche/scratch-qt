import QtQuick 2.0
import Ubuntu.Components 1.1
import "components"

MainView {
    id: main_view
    property string policy: ""
    // objectName for functional testing purposes (autopilot-qt5)
    objectName: "mainView"

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "com.ubuntu.developer.knitzsche.aptbrowser"

    //automaticOrientation: true

    useDeprecatedToolbar: false

    width: units.gu(160)
    height: units.gu(160)

    PageStack {
        id: pagestack
        Component.onCompleted: push(apt)
        Apt {
            id: apt
            objectName: "aptTab"
        }
    }
    function getPolicy(pkg) {
        details.apt_policy(pkg);
    }

}

