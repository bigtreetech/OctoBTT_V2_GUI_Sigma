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
    property bool moduleRun: root.parent.parent.state !== "default"
    onModuleRunChanged: {
        if (moduleRun === true) {
            console.debug(listView.count)
            listModel.clear()
            listView.count = 0
            functionModule.getCore()
        }
    }
    Connections {
        target: functionModule
        // @disable-check M16
        onCoreReply: {
            //json
            var metadata = JSON.parse(json)
            for (var data in metadata.array) {
                listView.count++
                listModel.insert(0, ({
                                         "inputJson": metadata.array[listView.count - 1]
                                     }))
                //                listModel.append({
                //                                     "inputJson": metadata.array[listView.count - 1]
                //                                 })
            }
        }
    }
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "transparent"
        border.color: borderColor
        border.width: borderWidth
        ListView {
            id: listView
            property int count: 0
            anchors.fill: parent
            orientation: ListView.Vertical
            clip: true
            model: listModel
            delegate: listDel
        }
        Component {
            id: listDel
            Rectangle {
                width: listView.width
                height: listView.height / listView.count
                color: "transparent"
                AllBtnStyle {
                    btnTextContent: inputJson.action
                    height: parent.height * 0.95
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.95
                    btnColor: "lightblue"
                    btnTextFontColor: "black"
                    onKeySignal: {
                        functionModule.runCore(inputJson.resource)
                    }
                }
            }
        }
        ListModel {
            id: listModel
        }
    }
}
