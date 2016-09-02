import QtQuick 2.2
import Ubuntu.Components 1.2
import Aptbrowser 1.0
import "../components"

Rectangle {
    id: cliRect
    property string cliResult: ""

    property int page_height: 120
    property int page_width: 160
    property int header_height: 12
    property int control_height: 10
    property int content_height: page_height - header_height - control_height
    property int status_height: 20
    property int deps_height: content_height
    property int content_height_with_status: content_height - status_height
    property bool cli_busy: false

    function getCli(cmd) {
        console.log("in getCli()");
        cli_busy = true;
        var res = cli.cli(cmd);
        console.log("res: " + res);
        status_label.text = res;
        status_label.height = units.gu(status_height);
        status.height = units.gu(status_height);
        status.visible = true;
    }

    Cli { // pulls in Cli c++ class
        id: cli
        onCliResultChanged: {
            console.log("==== cliChanged heard");
            cli_busy= false;
        }               
    }
    Rectangle {
        id: main
        width: units.gu(page_width)
        height: units.gu(page_height)
        Rectangle {
            id: row1
            height: units.gu(control_height/2)
            width: units.gu(parent.width)
            Rectangle {
                id: pkg_entry_row
                width: units.gu(parent.width)
                height: units.gu(control_height/2)
                function setPackage(pkg) {
                    command.remove(0,command.length)
                    command.paste(pkg);
                }
                TextField {
                    id: command
                    width: units.gu(page_width - 2)
                    onAccepted: {

                    }
                }
            }
        }
        Rectangle {
            id: cliButtons
            height: units.gu(control_height/2)
            width: units.gu(parent.width)
            anchors.top: row1.bottom
            Rectangle {
                id: cli_busy_id
                width: units.gu(2)
                height: units.gu(2)
                color: UbuntuColors.orange
                state: "NORMAL"
                RotationAnimator {
                    id: rotate
                    running: cli_busy
                    target: cli_busy_id
                    loops: 100000
                    from: 0
                    to: 360
                    duration: 50
                    onRunningChanged: {
                        cli_busy_id.color = UbuntuColors.purple;
                        cli_busy_id.state = "ROTATING";
                    }
                }
                states: [
                    State {
                        name: "NORMAL"
                        PropertyChanges { target: cli_busy_id; color: UbuntuColors.orange }
                    },
                    State {
                        name: "ROTATING"
                        PropertyChanges { target: cli_busy_id; color: UbuntuColors.purple }
                    }
                ]
            }
            Button {
                id: getCli_b
                anchors.left: cli_busy_id.right
                text: "CLI"
                onClicked: {
                    if (status.visible == true) {
                        hideStatus();
                    } else {
                        var cmd = command.getText(0,command.length);
                        getCli(cmd);
                    }
                }
            }
        }
        Rectangle {
            id: status
            anchors.top: cliButtons.bottom
            width: units.gu(page_width)
            height: units.gu(0)
            visible: false
            //color: UbuntuColors.lightGrey
            Label {
                id: status_label
                height: units.gu(status_height)
                text: "Status displays here"
                color: UbuntuColors.purple
            }
        }
    }
}
