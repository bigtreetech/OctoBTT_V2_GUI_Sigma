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
            id: msg_preview_area
            color: "transparent"
            width: parent.width * 0.95
            height: parent.height / 3 * 2 * 0.95
            anchors.topMargin: parent.height / 3 * 2 * 0.05 / 2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            Rectangle {
                id: fileName_area
                color: "#6a2e19"
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width / 2 * 0.975
                height: parent.height / 2 * 0.95
                border.color: "lightblue"
                radius: 5
                clip: true
                z: width === parent.width / 2 * 0.975 ? 0 : 1
                Text {
                    id: fileName
                    property var value: root.currentInfo.current.job.file.display
                    text: qsTr("File") + ": " + value
                    anchors.fill: parent
                    font.pointSize: height * 0.3
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                    color: "white"
                }
                Behavior on width {
                    NumberAnimation {
                        duration: 250
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.tip(fileName.text)
                    }
                    onPressed: {
                        parent.width = Qt.binding(function () {
                            return fileName.contentWidth + 4
                        })
                    }
                    onReleased: {
                        parent.width = Qt.binding(function () {
                            return parent.parent.width / 2 * 0.975
                        })
                    }
                }
            }
            Rectangle {
                id: estimated_area
                color: "#6a2e19"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width / 2 * 0.975
                height: parent.height / 2 * 0.95
                clip: true
                border.color: "lightblue"
                radius: 5
                z: width === parent.width / 2 * 0.975 ? 0 : 1
                Text {
                    id: estimated
                    property var estimatedPrintTime: root.currentInfo.current.job.estimatedPrintTime
                    property var days: (estimatedPrintTime / (3600 * 24)).toFixed(
                                           0)
                    property var hours: ((estimatedPrintTime - days * (3600 * 24)) / 3600).toFixed(
                                            0)
                    property var minutes: ((estimatedPrintTime - days * (3600 * 24)
                                            - hours * 3600) / 60).toFixed(0)
                    property var seconds: estimatedPrintTime - days * (3600 * 24)
                                          - hours * 3600 - minutes * 60
                    property var value: days + "d" + hours + ":" + minutes + ":" + seconds
                    text: qsTr("Estimate") + ": " + value
                    anchors.fill: parent
                    font.pointSize: height * 0.3
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                    color: "white"
                }
                Behavior on width {
                    NumberAnimation {
                        duration: 250
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.tip(estimated.value)
                    }
                    onPressed: {
                        parent.width = Qt.binding(function () {
                            return estimated.contentWidth + 4
                        })
                    }
                    onReleased: {
                        parent.width = Qt.binding(function () {
                            return parent.parent.width / 2 * 0.975
                        })
                    }
                }
            }
            Rectangle {
                id: completion_area
                color: "#6a2e19"
                anchors.right: parent.right
                anchors.top: parent.top
                width: parent.width / 2 * 0.975
                height: parent.height / 2 * 0.95
                clip: true
                border.color: "lightblue"
                radius: 5
                z: width === parent.width / 2 * 0.975 ? 0 : 1
                Text {
                    id: completion
                    property var value: root.currentInfo.current.progress.completion
                    text: qsTr("Complet") + ": " + value + "%"
                    anchors.fill: parent
                    font.pointSize: height * 0.3
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                    color: "white"
                }
                Behavior on width {
                    NumberAnimation {
                        duration: 250
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.tip(completion.text)
                    }
                    onPressed: {
                        parent.width = Qt.binding(function () {
                            return completion.contentWidth + 4
                        })
                    }
                    onReleased: {
                        parent.width = Qt.binding(function () {
                            return parent.parent.width / 2 * 0.975
                        })
                    }
                }
            }
            Rectangle {
                id: timeLeft_area
                color: "#6a2e19"
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: parent.width / 2 * 0.975
                height: parent.height / 2 * 0.95
                clip: true
                border.color: "lightblue"
                radius: 5
                z: width === parent.width / 2 * 0.975 ? 0 : 1
                Text {
                    id: timeLeft
                    property var printTimeLeft: root.currentInfo.current.job.printTimeLeft
                    property var days: (printTimeLeft / (3600 * 24)).toFixed(0)
                    property var hours: ((printTimeLeft - days * (3600 * 24)) / 3600).toFixed(
                                            0)
                    property var minutes: ((printTimeLeft - days * (3600 * 24)
                                            - hours * 3600) / 60).toFixed(0)
                    property var seconds: printTimeLeft - days * (3600 * 24)
                                          - hours * 3600 - minutes * 60
                    property var value: days + "d" + hours + ":" + minutes + ":" + seconds
                    text: qsTr("TimeLeft") + ": " + value
                    anchors.fill: parent
                    font.pointSize: height * 0.3
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                    color: "white"
                }
                Behavior on width {
                    NumberAnimation {
                        duration: 250
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.tip(timeLeft.value)
                    }
                    onPressed: {
                        parent.width = Qt.binding(function () {
                            return timeLeft.contentWidth + 4
                        })
                    }
                    onReleased: {
                        parent.width = Qt.binding(function () {
                            return parent.parent.width / 2 * 0.975
                        })
                    }
                }
            }
        }
        Rectangle {
            id: control_area
            color: "transparent"
            width: parent.width * 0.95
            height: parent.height / 3 * 0.95
            anchors.bottomMargin: parent.height / 3 * 0.05 / 2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            Rectangle {
                id: babystep_dec_area
                color: "transparent"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                width: parent.width / 4 * 0.95
                height: parent.height * 0.95
                AllBtnStyle {
                    btnImagePath: "../assets/arrow_down.svg"
                    anchors.fill: parent
                    btnColor: "lightgray"
                    anchors.rightMargin: parent.height * 0.05
                    anchors.leftMargin: parent.height * 0.05
                    anchors.bottomMargin: parent.width * 0.025
                    anchors.topMargin: parent.width * 0.025
                    onKeySignal: {
                        functionModule.sentGcodeCommand(
                                    ["G91", "M290 Z-0.01", "M500", "G90"])
                    }
                }
            }
            Rectangle {
                id: babystep_add_area
                color: "transparent"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width / 4 * 0.95
                height: parent.height * 0.95
                AllBtnStyle {
                    btnImagePath: "../assets/arrow_up.svg"
                    anchors.fill: parent
                    btnColor: "lightgray"
                    anchors.rightMargin: parent.height * 0.05
                    anchors.leftMargin: parent.height * 0.05
                    anchors.bottomMargin: parent.width * 0.025
                    anchors.topMargin: parent.width * 0.025
                    onKeySignal: {
                        functionModule.sentGcodeCommand(
                                    ["G91", "M290 Z0.01", "M500", "G90"])
                    }
                }
            }
            Rectangle {
                id: job_control_area
                color: "transparent"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width / 2 * 0.95
                height: parent.height * 0.95
                AllBtnStyle {
                    btnTextContent: root.currentInfo.current.state.flags.operational
                                    && !root.currentInfo.current.state.flags.printing && !root.currentInfo.current.state.flags.cancelling
                                    && !root.currentInfo.current.state.flags.pausing && !root.currentInfo.current.state.flags.resuming
                                    && !root.currentInfo.current.state.flags.finishing ? qsTr("Print") : root.currentInfo.current.state.flags.operational && root.currentInfo.current.state.flags.printing && !root.currentInfo.current.state.flags.cancelling && !root.currentInfo.current.state.flags.pausing && !root.currentInfo.current.state.flags.resuming && !root.currentInfo.current.state.flags.finishing ? qsTr("Stop") : qsTr("---")
                    enabled: root.currentInfo.current.state.flags.operational
                             && root.currentInfo.current.job.file.path !== null
                             && btnTextContent !== qsTr("---")
                    height: parent.height * 0.95
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.95
                    btnColor: "lightblue"
                    btnTextFontColor: "black"
                    onKeySignal: {
                        if (btnTextContent === qsTr("Print")) {
                            functionModule.sentJobControl("start")
                        } else if (btnTextContent === qsTr("Stop")) {
                            functionModule.sentJobControl("cancel")
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:9;anchors_height:72.2}
}
##^##*/

