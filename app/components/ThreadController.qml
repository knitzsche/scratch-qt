import QtQuick 2.2
import Ubuntu.Components 1.2
import  Scratchqt 1.0
import "../components"

Rectangle  {
    id: threads

    Job {
        id: job1
        button_text: "Job 1"
        job: "job1"
        anchors.top: parent.top
    }
    Job {
        id: job2
        job: "job2"
        button_text: "Job 2"
        anchors.top: job1.bottom 
    }
    Job {
        id: job3
        job: "job3"
        button_text: "Job 3"
        anchors.top: job2.bottom 
    }
    Job {
        id: job4
        job: "job4"
        button_text: "Job 4"
        anchors.top: job3.bottom 
    }
}
