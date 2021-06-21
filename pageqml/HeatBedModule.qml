import QtQuick 2.9
import QtQuick.Controls 2.2
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
    property var currentInfo
    Connections {
        target: myWebsoket
        // @disable-check M16
        onWsCurrentInfo: {
            root.getcurrentInfo(json)
        }
    }
    function getcurrentInfo(jsonpacket) {
        root.currentInfo = JSON.parse(jsonpacket)
    }

    Rectangle {
        id: rect
        anchors.fill: parent
        color: "transparent"
        border.color: borderColor
        border.width: borderWidth

        Rectangle {
            //预览区
            id: heatbedPreview_area
            height: parent.height / 4
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            color: "transparent"
            Rectangle {
                id: heatbed_actual_area
                width: parent.width / 2
                height: parent.height
                color: "transparent"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: heatbed_actual
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width * 0.05
                    anchors.verticalCenter: parent.verticalCenter
                    width: contentWidth
                    text: root.currentInfo.current.state.flags.operational
                          === true ? qsTr("Actual") + " : "
                                     + root.currentInfo.current.temps[0].bed.actual + "℃" : qsTr(
                                         "Actual")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: height * 0.3
                }
            }
            Rectangle {
                id: heatbed_target_area
                width: parent.width / 2
                height: parent.height
                color: "transparent"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: heatbed_target
                    height: parent.height
                    anchors.right: parent.right
                    anchors.rightMargin: parent.width * 0.05
                    anchors.verticalCenter: parent.verticalCenter
                    width: contentWidth
                    text: root.currentInfo.current.state.flags.operational
                          === true ? qsTr("Target") + " : "
                                     + root.currentInfo.current.temps[0].bed.target + "℃" : qsTr(
                                         "Target")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: height * 0.3
                }
            }
        }
        Rectangle {
            //配置区
            id: heatbedSetting_area
            height: parent.height / 2
            anchors.top: heatbedPreview_area.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            color: "transparent"
            Rectangle {
                //降低按钮
                id: dec_Btn_area
                width: parent.width / 5
                height: parent.height
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                color: "transparent"
                AllBtnStyle {
                    btnImagePath: "../assets/KB/left.svg"
                    anchors.fill: parent
                    repeaterTimer: true
                    btnColor: "lightgray"
                    anchors.rightMargin: parent.width * 0.05
                    anchors.leftMargin: parent.width * 0.05
                    anchors.bottomMargin: parent.height * 0.05
                    anchors.topMargin: parent.height * 0.05
                    onKeySignal: {
                        if (numEdit.select) {
                            if (numEdit.value > 0)
                                numEdit.value--
                        } else {
                            if (maxEdit.value > 0)
                                maxEdit.value--
                            if (numEdit.value > maxEdit.value)
                                numEdit.value = maxEdit.value
                        }
                    }
                }
            }
            Rectangle {
                id: num_area
                height: parent.height
                width: parent.width / 5 * 3
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "transparent"
                clip: true
                Rectangle {
                    //上限设置区
                    id: maxEdit_area
                    height: parent.height / 5
                    anchors.top: parent.top
                    width: maxEdit.width + 4 * border.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    border.width: !numEdit.select ? 2 : 0
                    border.color: !numEdit.select ? "lime" : "transparent"
                    radius: 5
                    Behavior on border.color {
                        ColorAnimation {
                            duration: 500
                        }
                    }
                    Behavior on border.width {
                        NumberAnimation {
                            duration: 500
                        }
                    }
                    Text {
                        id: maxEdit
                        property int value: 80
                        text: value + " MAX"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        height: parent.height
                        width: contentWidth
                        Behavior on width {
                            NumberAnimation {
                                duration: 100
                            }
                        }
                        font.pointSize: height * 0.5
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                numEdit.select = false
                            }
                        }
                    }
                }
                Rectangle {
                    //设定值设置区
                    id: numEdit_area
                    height: parent.height / 5 * 3 * 0.95
                    width: numEdit.width + 4 * border.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: "transparent"
                    border.width: numEdit.select ? 2 : 0
                    border.color: numEdit.select ? "lime" : "transparent"
                    Behavior on border.color {
                        ColorAnimation {
                            duration: 500
                        }
                    }
                    Behavior on border.width {
                        NumberAnimation {
                            duration: 500
                        }
                    }
                    radius: 5
                    Text {
                        id: numEdit
                        property int value: 0
                        property bool select: true
                        text: value + "℃"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        height: parent.height
                        width: contentWidth
                        font.pointSize: height * 0.7
                        Behavior on width {
                            NumberAnimation {
                                duration: 100
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                numEdit.select = true
                            }
                        }
                    }
                }
            }
            Rectangle {
                //增加按钮
                id: add_Btn_area
                width: parent.width / 5
                height: parent.height
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                color: "transparent"
                AllBtnStyle {
                    btnImagePath: "../assets/KB/right.svg"
                    anchors.fill: parent
                    repeaterTimer: true
                    btnColor: "lightgray"
                    anchors.rightMargin: parent.width * 0.05
                    anchors.leftMargin: parent.width * 0.05
                    anchors.bottomMargin: parent.height * 0.05
                    anchors.topMargin: parent.height * 0.05
                    onKeySignal: {
                        if (numEdit.select) {
                            if (numEdit.value < maxEdit.value)
                                numEdit.value++
                        } else {
                            maxEdit.value++
                        }
                    }
                }
            }
        }
        Rectangle {
            //按钮确认区
            id: heatbedBtn_area
            height: parent.height / 4
            anchors.top: heatbedSetting_area.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            color: "transparent"
            AllBtnStyle {
                btnTextContent: qsTr("Off")
                height: parent.height * 0.9
                anchors.left: parent.left
                anchors.leftMargin: width / 0.95 * 0.05
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width / 3 * 0.95
                btnColor: "lightgray"
                btnTextFontColor: "black"
                onKeySignal: {
                    numEdit.value = 0
                    functionModule.sentGcodeCommand("M140 S" + numEdit.value)
                }
            }
            AllBtnStyle {
                btnTextContent: qsTr("Set")
                height: parent.height * 0.95
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width / 3 * 0.95
                btnColor: "lightgray"
                btnTextFontColor: "black"
                onKeySignal: {
                    functionModule.sentGcodeCommand("M140 S" + numEdit.value)
                }
            }
            AllBtnStyle {
                btnTextContent: qsTr("Full")
                height: parent.height * 0.9
                anchors.rightMargin: width / 0.95 * 0.05
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                width: parent.width / 3 * 0.95
                btnColor: "lightgray"
                btnTextFontColor: "black"
                onKeySignal: {
                    numEdit.value = maxEdit.value
                    functionModule.sentGcodeCommand("M140 S" + numEdit.value)
                }
            }
        }
    }
}
