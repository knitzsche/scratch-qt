import QtQuick 2.2
import Ubuntu.Components 1.2
import  Aptbrowser 1.0
import "../components"

Rectangle  {
    id: threads

    ThreadWorker {
        id: worker1
        button_text: "Thread 1"
        anchors.top: parent.top
    }
    ThreadWorker {
        id: worker2
        button_text: "Thread 2"
        anchors.top: worker1.bottom 
    }
}
