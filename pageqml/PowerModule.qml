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
        } else {
            if (question.fuzzy === true)
                question.select(false)
        }
    }
    Connections {
        target: functionModule
        // @disable-check M16
        onCoreReply: {
            //json
            var metadata = JSON.parse(json)
            for (var core in metadata.core) {
                listView.count++
                listModel.insert(0, ({
                                         "inputJson": metadata.core[listView.count - 1]
                                     }))
            }
            for (var custom in metadata.custom) {
                listView.count++
                listModel.insert(0, ({
                                         "inputJson": metadata.custom[listView.count - 1]
                                     }))
            }
        }
    }
    Glass {
        id: question
        fuzzy: false
        glasstraget: rect
        signal select(var check)
        signal show(var themeText)
        property alias text: contentText.text
        enabled: fuzzy
        onShow: {
            text = themeText
            question.fuzzy = true
        }
        onSelect: {
            question.fuzzy = false
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
        }

        Text {
            id: contentText
            visible: question.fuzzy
            height: parent.height / 2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: (height / 5) <= 0 ? 1 : (height / 5)
            color: "black"
            font.bold: true
        }

        AllBtnStyle {
            visible: question.fuzzy
            btnTextContent: qsTr("Yes")
            height: parent.height / 4
            width: parent.width / 3
            anchors.bottomMargin: height / 2
            anchors.leftMargin: width / 3
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            btnColor: "red"
            btnTextFontColor: "yellow"
            onKeySignal: {
                question.select(true)
            }
        }
        AllBtnStyle {
            visible: question.fuzzy
            btnTextContent: qsTr("No")
            height: parent.height / 4
            width: parent.width / 3
            anchors.bottomMargin: height / 2
            anchors.rightMargin: width / 3
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            btnColor: "darkgreen"
            btnTextFontColor: "lightgreen"
            onKeySignal: {
                question.select(false)
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
                        question.select.connect(rec)
                        question.show(inputJson.action + " ?")
                    }
                    function rec(check) {
                        console.debug("Test: " + check)
                        if (check)
                            functionModule.runCore(inputJson.resource)
                        question.select.disconnect(rec)
                    }
                }
            }
        }
        ListModel {
            id: listModel
        }
    }
}
