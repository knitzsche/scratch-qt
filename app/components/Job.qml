import QtQuick 2.2
import Ubuntu.Components 1.1
import Aptbrowser 1.0

ThreadWorker {
    id: w1
    property bool run_busy: false
    property  string button_text: "default"
    height: units.gu(5)
    function runMe() {
        console.log("in run 1 ITEM");
        run_busy = true;
        threader.state = "ROTATING";
        worker1.start_me();
        console.log("done with run 2 ITEM");
    }
    Rectangle {
        id: threader 
        width: units.gu(2)
        height: units.gu(2)
        anchors.top: parent.top
        anchors.topMargin: units.gu(1)
        anchors.left: parent.left
        anchors.leftMargin: units.gu(1)
        color: UbuntuColors.orange
        state: "NORMAL"
        RotationAnimator {
            id: rotate
            running: run_busy
            target: threader 
            loops: 100000
            from: 0
            to: 360
            duration: 50
            onRunningChanged: {
                threader.color = UbuntuColors.purple;
                threader.state = "ROTATING";
            }
        }
        states: [
            State {
                name: "NORMAL"
                PropertyChanges { target: threader; color: UbuntuColors.orange }
            },
            State {
                name: "ROTATING"
                PropertyChanges { target: threader; color: UbuntuColors.purple }
            }
        ]
    }
    Button {
        id: thread_b
        anchors.left: threader.right
        anchors.leftMargin: 10
        text: button_text
        color: UbuntuColors.midAubergine
        onClicked: {
            console.log(text  + " clicked");
            runMe(); 
        }
    }
    onRunDone: {
        console.log("run done in ITEM");
        run_busy = false;
        threader.state = "NORMAL";        

    }
}
