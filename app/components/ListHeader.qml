import QtQuick 2.0
import Ubuntu.Components 1.1

Rectangle {
    color: UbuntuColors.blue
    property string text: header_text.text
    function setText (txt) {
        header_text.text = txt;
    }

    Text {
        id: header_text
        color: UbuntuColors.darkAubergine
        font.bold: true
    }
}

