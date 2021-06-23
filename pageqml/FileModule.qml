import QtQuick 2.9
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

Item {
    property string file_version: "1.0.0.0" //文件版本控制
    id: root
    /*普通提醒*/
    signal tip(var outputText)
    /*强提醒*/
    signal reminderTip(var outputText)
    property color borderColor
    property double borderWidth
    property bool run: root.parent.root.workArea.state !== "default"

    property var fileOctoTree
    Connections {
        target: myWebsoket
        // @disable-check M16
        onOctoFileReply: {
            root.fileOctoTree = JSON.parse(json)
        }
    }
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "lightblue"
        border.color: borderColor
        border.width: borderWidth
    }
}
