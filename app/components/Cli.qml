import QtQuick 2.5
import Ubuntu.Components 1.2
import  QtQuick.Layouts 1.2
import Aptbrowser 1.0
import "../components"

Rectangle {
    id: cliRect
    property string cliResult: ""
    property string cmd_snap_list: "snap list";
    property string cmd: command.text
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
 
    focus: true
    Keys.onPressed: {
        if (event.key == Qt.Key_Return){
            console.log("== RETURN pressed");
            getCli(cmd);
        }
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
            anchors.top: main.top
            Rectangle {
                id: pkg_entry_rownhc
                width: units.gu(parent.width)
                height: units.gu(control_height/2)
                anchors.top: row1.top
                TextField {
                    id: command
                    width: units.gu(page_width - 2)
                    anchors.top: pkg_entry_rownhc.top
                    placeholderText: "Enter command"
                }
            }
        }
        Rectangle {
            id: row2
            anchors.top: row1.bottom

            Rectangle {
                id: cliButtons
                anchors.top: row2.top
                anchors.left: row2.left
                width: 150
                height: 200
                Rectangle {
                    id: cli_busy_id
                    width: units.gu(2)
                    height: units.gu(2)
                    Button {
                        id: getCli_b
                        anchors.top: cli_busy_id.top
                        text: "Your Command"
                        onClicked: {
                            getCli(cmd);
                        }
                    }
                    Button {
                        id: cmd1
                        anchors.top: getCli_b.bottom
                        anchors.topMargin: 5
                        text: cmd_snap_list
                        onClicked: {
                            getCli(cmd_snap_list);
                        }
                    }
                    Button {
                        id: cmd3
                        text: "snap interfaces"
                        anchors.topMargin: 5
                        anchors.top: getCli_b.bottom
                        onClicked: {
                            getCli("snap interfaces");
                        }
                    }
                    Button {
                        id: cmd_env
                        text: "env"
                        anchors.top: cmd3.bottom
                        anchors.topMargin: 5
                        onClicked: {
                            getCli("env");
                        }
                    }
                    Button {
                        id: cmd_env_snap
                        text: "env SNAP"
                        anchors.top: cmd_env.bottom
                        anchors.topMargin: 5
                        onClicked: {
                            getCli("env | grep SNAP");
                        }
                    }
               }
            }
            Rectangle {
                id: status
                anchors.top: parent.top
                anchors.left: cliButtons.right
                width: 50
                height: 200
                Text {
                    id: status_label
                    height: 200
                    font.family: "Monospace"
                    color: UbuntuColors.purple
                }
            }
        }
    }
}
