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
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "lightblue"
        border.color: borderColor
        border.width: borderWidth
        Rectangle {
            id: top_area
            width: parent.width - 2 * parent.border.width
            height: (parent.height - 2 * parent.border.width) / 4 * 3
            anchors.top: parent.top
            anchors.topMargin: parent.border.width
            anchors.horizontalCenter: parent.horizontalCenter
            color: "transparent"
            Rectangle {
                id: left_area
                width: parent.width / 4 * 3
                height: parent.height
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                color: "darkgreen"
                Rectangle {
                    id: x_area
                    width: parent.width
                    height: parent.height / 3
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    AllBtnStyle {
                        btnTextContent: "-"
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        btnColor: "lightgreen"
                        btnTextFontColor: "black"
                        height: parent.height * 0.9
                        anchors.leftMargin: parent.width * 0.025
                        width: parent.width / 3 * 0.95
                        onKeySignal: {
                            functionModule.sentGcodeCommand(
                                        ["G91", "G1 X-" + numEdit.value, "G90"])
                        }
                    }
                    Text {
                        height: parent.height * 0.95
                        width: parent.width / 3 * 0.95
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("X")
                        font.pointSize: height * 0.7
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    AllBtnStyle {
                        btnTextContent: "+"
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        btnColor: "lightgreen"
                        btnTextFontColor: "black"
                        height: parent.height * 0.9
                        anchors.rightMargin: parent.width * 0.025
                        width: parent.width / 3 * 0.95
                        onKeySignal: {
                            functionModule.sentGcodeCommand(
                                        ["G91", "G1 X" + numEdit.value, "G90"])
                        }
                    }
                }
                Rectangle {
                    id: y_area
                    width: parent.width
                    height: parent.height / 3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    AllBtnStyle {
                        btnTextContent: "-"
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        btnColor: "lightgreen"
                        btnTextFontColor: "black"
                        height: parent.height * 0.9
                        anchors.leftMargin: parent.width * 0.025
                        width: parent.width / 3 * 0.95
                        onKeySignal: {
                            functionModule.sentGcodeCommand(
                                        ["G91", "G1 Y-" + numEdit.value, "G90"])
                        }
                    }
                    Text {
                        height: parent.height * 0.95
                        width: parent.width / 3 * 0.95
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Y")
                        font.pointSize: height * 0.7
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    AllBtnStyle {
                        btnTextContent: "+"
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        btnColor: "lightgreen"
                        btnTextFontColor: "black"
                        height: parent.height * 0.9
                        anchors.rightMargin: parent.width * 0.025
                        width: parent.width / 3 * 0.95
                        onKeySignal: {
                            functionModule.sentGcodeCommand(
                                        ["G91", "G1 Y" + numEdit.value, "G90"])
                        }
                    }
                }
                Rectangle {
                    id: z_area
                    width: parent.width
                    height: parent.height / 3
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    AllBtnStyle {
                        btnTextContent: "-"
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        btnColor: "lightgreen"
                        btnTextFontColor: "black"
                        height: parent.height * 0.9
                        anchors.leftMargin: parent.width * 0.025
                        width: parent.width / 3 * 0.95
                        onKeySignal: {
                            functionModule.sentGcodeCommand(
                                        ["G91", "G1 Z-" + numEdit.value, "G90"])
                        }
                    }
                    Text {
                        height: parent.height * 0.95
                        width: parent.width / 3 * 0.95
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Z")
                        font.pointSize: height * 0.7
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    AllBtnStyle {
                        btnTextContent: "+"
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        btnColor: "lightgreen"
                        btnTextFontColor: "black"
                        height: parent.height * 0.9
                        anchors.rightMargin: parent.width * 0.025
                        width: parent.width / 3 * 0.95
                        onKeySignal: {
                            functionModule.sentGcodeCommand(
                                        ["G91", "G1 Z" + numEdit.value, "G90"])
                        }
                    }
                }
            }
            Rectangle {
                id: right_area
                width: parent.width / 4
                height: parent.height
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                color: "orange"
                Rectangle {
                    id: value_add_area
                    width: parent.width
                    height: parent.height / 3
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    AllBtnStyle {
                        btnImagePath: "../assets/arrow_up.svg"
                        anchors.fill: parent
                        repeaterTimer: true
                        btnColor: "blue"
                        anchors.rightMargin: parent.width * 0.05
                        anchors.leftMargin: parent.width * 0.05
                        anchors.bottomMargin: parent.height * 0.1
                        anchors.topMargin: parent.height * 0.1
                        onKeySignal: {
                            if (numEdit.value < 50)
                                numEdit.value++
                        }
                    }
                }
                Rectangle {
                    id: value_area
                    width: parent.width
                    height: parent.height / 3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    Text {
                        id: numEdit
                        property int value: 10
                        text: value
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
                    }
                }
                Rectangle {
                    id: value_dec_area
                    width: parent.width
                    height: parent.height / 3
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    AllBtnStyle {
                        btnImagePath: "../assets/arrow_down.svg"
                        anchors.fill: parent
                        repeaterTimer: true
                        btnColor: "blue"
                        anchors.rightMargin: parent.width * 0.05
                        anchors.leftMargin: parent.width * 0.05
                        anchors.bottomMargin: parent.height * 0.1
                        anchors.topMargin: parent.height * 0.1
                        onKeySignal: {
                            if (numEdit.value > 1)
                                numEdit.value--
                        }
                    }
                }
            }
        }
        Rectangle {
            id: bottom_area
            width: parent.width - 2 * parent.border.width
            height: (parent.height - 2 * parent.border.width) / 4
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.border.width
            anchors.horizontalCenter: parent.horizontalCenter
            color: "darkgray"
            Rectangle {
                id: x_home_area
                width: parent.width / 3 * 0.95
                height: parent.height
                anchors.left: parent.left
                anchors.leftMargin: parent.width / 3 * 0.025
                anchors.verticalCenter: parent.verticalCenter
                color: "transparent"
                AllBtnStyle {
                    btnTextContent: ">X<"
                    anchors.fill: parent
                    btnColor: "lightgray"
                    btnTextFontColor: "black"
                    anchors.rightMargin: parent.width * 0.05
                    anchors.leftMargin: parent.width * 0.05
                    anchors.bottomMargin: parent.height * 0.1
                    anchors.topMargin: parent.height * 0.1
                    onKeySignal: {
                        functionModule.sentGcodeCommand(["G91", "G28 X", "G90"])
                    }
                }
            }
            Rectangle {
                id: y_home_area
                width: parent.width / 3 * 0.95
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "transparent"
                AllBtnStyle {
                    btnTextContent: ">Y<"
                    anchors.fill: parent
                    btnColor: "lightgray"
                    btnTextFontColor: "black"
                    anchors.rightMargin: parent.width * 0.05
                    anchors.leftMargin: parent.width * 0.05
                    anchors.bottomMargin: parent.height * 0.1
                    anchors.topMargin: parent.height * 0.1
                    onKeySignal: {
                        functionModule.sentGcodeCommand(["G91", "G28 Y", "G90"])
                    }
                }
            }
            Rectangle {
                id: z_home_area
                width: parent.width / 3 * 0.95
                height: parent.height
                anchors.right: parent.right
                anchors.rightMargin: parent.width / 3 * 0.025
                anchors.verticalCenter: parent.verticalCenter
                color: "transparent"
                AllBtnStyle {
                    btnTextContent: ">Z<"
                    anchors.fill: parent
                    btnColor: "lightgray"
                    btnTextFontColor: "black"
                    anchors.rightMargin: parent.width * 0.05
                    anchors.leftMargin: parent.width * 0.05
                    anchors.bottomMargin: parent.height * 0.1
                    anchors.topMargin: parent.height * 0.1
                    onKeySignal: {
                        functionModule.sentGcodeCommand(["G91", "G28 Z", "G90"])
                    }
                }
            }
        }
    }
}
