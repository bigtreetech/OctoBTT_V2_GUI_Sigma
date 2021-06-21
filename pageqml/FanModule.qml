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

    Rectangle {
        id: rect
        anchors.fill: parent
        color: "transparent"
        border.color: borderColor
        border.width: borderWidth

        Rectangle {
            //预览区
            id: fanPreview_area
            height: parent.height / 4
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            color: "transparent"
            AllBtnStyle {
                btnTextContent: qsTr("Half")
                height: parent.height * 0.9
                anchors.left: parent.left
                anchors.leftMargin: width / 0.95 * 0.05 / 2
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width / 2 * 0.95
                btnColor: "lightgray"
                btnTextFontColor: "black"
                onKeySignal: {
                    numEdit.value = 128
                    functionModule.sentGcodeCommand("M106 S" + numEdit.value)
                }
            }
            AllBtnStyle {
                btnTextContent: qsTr("Full")
                height: parent.height * 0.9
                anchors.rightMargin: width / 0.95 * 0.05 / 2
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                width: parent.width / 2 * 0.95
                btnColor: "lightgray"
                btnTextFontColor: "black"
                onKeySignal: {
                    numEdit.value = 255
                    functionModule.sentGcodeCommand("M106 S" + numEdit.value)
                }
            }
        }
        Rectangle {
            //配置区
            id: fanSetting_area
            height: parent.height / 2
            anchors.top: fanPreview_area.bottom
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
                    anchors.rightMargin: parent.height * 0.05
                    anchors.leftMargin: parent.height * 0.05
                    anchors.bottomMargin: parent.width * 0.05
                    anchors.topMargin: parent.width * 0.05
                    onKeySignal: {
                        if (numEdit.value > 0)
                            numEdit.value--
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
                    //实际值预览区
                    id: numView_area
                    height: parent.height / 5
                    anchors.top: parent.top
                    width: numView.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "transparent"
                    Text {
                        id: numView
                        text: numEdit.value + " / 255"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        height: parent.height
                        width: contentWidth
                        font.pointSize: height * 0.5
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
                    border.width: 2
                    border.color: "yellow"
                    radius: 5
                    Text {
                        id: numEdit
                        property int value: 0
                        text: ((value / 255)
                               === 1 ? "100" : ((value / 255)
                                                >= 0.1 ? ((value / 255) * 100).toFixed(
                                                             1) : ((value / 255) * 100).toFixed(
                                                             2))) + "%"
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
                    anchors.rightMargin: parent.height * 0.05
                    anchors.leftMargin: parent.height * 0.05
                    anchors.bottomMargin: parent.width * 0.05
                    anchors.topMargin: parent.width * 0.05
                    onKeySignal: {
                        if (numEdit.value < 255)
                            numEdit.value++
                    }
                }
            }
        }
        Rectangle {
            //按钮确认区
            id: fanBtn_area
            height: parent.height / 4
            anchors.top: fanSetting_area.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            color: "transparent"
            AllBtnStyle {
                btnTextContent: qsTr("Off")
                height: parent.height * 0.9
                anchors.left: parent.left
                anchors.leftMargin: width / 0.95 * 0.05 / 2
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width / 2 * 0.95
                btnColor: "lightgray"
                btnTextFontColor: "black"
                onKeySignal: {
                    numEdit.value = 0
                    functionModule.sentGcodeCommand("M106 S" + numEdit.value)
                }
            }
            AllBtnStyle {
                btnTextContent: qsTr("Set")
                height: parent.height * 0.9
                anchors.rightMargin: width / 0.95 * 0.05 / 2
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                width: parent.width / 2 * 0.95
                btnColor: "lightgray"
                btnTextFontColor: "black"
                onKeySignal: {
                    functionModule.sentGcodeCommand("M106 S" + numEdit.value)
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

