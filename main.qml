import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQml 2.0
import QtQuick.Window 2.2
// import "qrc:/pageqml"
// import "qrc:/moduleqml"
import "./pageqml"
import "./moduleqml"

ApplicationWindow {
    property string file_version: "1.0.0.0" //文件版本控制
    id: windows
    property double previewitemsProportion: 0.25 //中间元素垂直比例
    property int limit_size: 100 //元素垂直大小限制
    //	width: Screen.desktopAvailableWidth
    //	height: Screen.desktopAvailableHeight
    width: 480
    height: 288
    minimumWidth: 480
    minimumHeight: 288
    visible: true
    color: "gray"
    title: "OctoBTT V2"
    Component.onCompleted: {
        myWebsoket.webSocketBegin()
    }

    Connections {
        target: myWebsoket
        // @disable-check M16
        onSettingsUpdatedEvent: {
            functionModule.getSettings()
        }
    }
    Item {
        id: mian_item
        anchors.fill: parent
        //默认状态机为"默认"
        state: "default"
        //焦点截取
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            anchors.centerIn: parent
            z: 0
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    parent.focus = true
                }
            }
            onFocusChanged: {

            }
        }
        ///毛玻璃效果
        Glass {
            id: glass
            fuzzy: false
            glasstraget: main_page
        }
        ///主页
        Rectangle {
            id: main_page
            height: parent.height
            width: parent.width
            x: parent.x
            y: parent.y
            z: 1
            color: "#222222"
            Page_Main {
                onClick_GTerminal: mian_item.state = "GTerminal"
                onLoginfrm: {
                    login_page.state = "Show"
                }
                onTip: tip_page.setTip(outputText)
                onReminderTip: tip_page.setReminder(outputText)
                onSender: {
                    switch (data) {
                    case "ACLogin":
                        login_page.state = "Show"
                        break
                    default:
                        break
                    }
                }
            }
        }
        ///控制台
        Rectangle {
            id: gTerminal_page
            height: parent.height
            width: parent.width
            anchors.top: main_page.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            z: 1
            color: "gray"
            Page_GTerminal {
                onClick_back: mian_item.state = "default"
                onClick_keyboard: {
                    keyboard_page.exit.connect(receiver)
                    keyboard_page.inputData(inputdata)
                }
                function receiver(data, result) {
                    setText(data, result)
                    keyboard_page.exit.disconnect(receiver)
                }
                onClick_send: {
                    tip_page.setTip(data)
                }
            }
        }
        ///键盘页面
        Rectangle {
            id: keyboard_page
            height: parent.height
            width: parent.width
            x: parent.x
            y: height
            z: glass.z + 2
            color: "transparent"
            opacity: 0
            signal inputData(var content)
            signal exit(var outputdata, var checkState)
            onInputData: {
                keyTest.inputText(content)
                state = "Show"
            }
            BKeyboard {
                id: keyTest
                z: 1
                onExit: {
                    keyboard_page.state = "default"
                    parent.exit(content, btnState)
                }
            }
            states: [
                State {
                    name: "Show"
                    PropertyChanges {
                        target: keyboard_page
                        y: mian_item.y
                        opacity: 1
                    }
                    PropertyChanges {
                        target: login_page
                        fuzzy: true
                    }
                    PropertyChanges {
                        target: glass
                        fuzzy: true
                    }
                }
            ]
            transitions: [
                Transition {
                    to: "Show"
                    ParallelAnimation {
                        NumberAnimation {
                            target: keyboard_page
                            properties: "y"
                            to: mian_item.y
                            duration: 300
                            easing.type: Easing.InOutExpo
                        }
                        NumberAnimation {
                            target: keyboard_page
                            properties: "opacity"
                            to: 1
                            duration: 300
                            easing.type: Easing.OutSine
                        }
                    }
                },
                Transition {
                    to: "default"
                    ParallelAnimation {
                        NumberAnimation {
                            target: keyboard_page
                            properties: "y"
                            to: height
                            duration: 300
                            easing.type: Easing.InOutExpo
                        }
                        NumberAnimation {
                            target: keyboard_page
                            properties: "opacity"
                            to: 0
                            duration: 300
                            easing.type: Easing.OutSine
                        }
                    }
                }
            ]
        }
        ///提醒窗
        Rectangle {
            id: tip_page
            height: parent.height
            width: parent.width
            x: 0
            y: 0
            z: glass.z + 3
            color: "transparent"
            opacity: 0
            state: "default"
            enabled: state != "default"
            function setTip(inputText) {
                tip_text.text = qsTr(inputText)
                state = "Show"
            }
            function setReminder(inputText) {
                tip_text.text = qsTr(inputText)
                state = "Reminder"
            }
            Text {
                id: tip_text
                anchors.leftMargin: parent.width * 0.1
                anchors.rightMargin: parent.width * 0.1
                anchors.topMargin: parent.height * 0.3
                anchors.bottomMargin: parent.height * 0.3
                anchors.fill: parent

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: (height / 5) <= 0 ? 1 : (height / 5)
                color: "#FFFFFF"
                wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                Behavior on color {
                    ColorAnimation {
                        duration: 600
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    parent.state = "default"
                }
                hoverEnabled: true
            }
            ///States Group
            states: [
                State {
                    name: "Reminder"
                    PropertyChanges {
                        target: tip_page
                        opacity: 1
                    }
                    PropertyChanges {
                        target: glass
                        fuzzy: true
                    }
                    PropertyChanges {
                        target: tip_text
                        color: "#FF0000"
                    }
                    PropertyChanges {
                        target: login_page
                        fuzzy: true
                    }
                },
                State {
                    name: "Show"
                    PropertyChanges {
                        target: tip_page
                        opacity: 1
                    }
                    PropertyChanges {
                        target: glass
                        fuzzy: true
                    }
                    PropertyChanges {
                        target: login_page
                        fuzzy: true
                    }
                },
                State {
                    name: "default"
                    PropertyChanges {
                        target: tip_page
                        opacity: 0
                    }
                    PropertyChanges {
                        target: tip_text
                        color: "#FFFFFF"
                    }
                }
            ]
            ///Animation
            Behavior on opacity {
                NumberAnimation {
                    duration: 300
                }
            }
        }
        ///登陆页面
        Rectangle {
            id: login_page
            height: parent.height
            width: parent.width
            x: parent.x
            y: parent.y - height
            z: glass.z + 1
            color: "transparent"
            opacity: 0
            state: "default"
            property alias fuzzy: page_Login.fuzzy
            Page_Login {
                id: page_Login
                z: 1
                onExit: {
                    login_page.state = "default"
                }
                onCallKeyboard: {
                    keyboard_page.exit.connect(receiver)
                    keyboard_page.inputData(inputdata)
                }
                function receiver(data, result) {
                    setText(data, result)
                    console.debug("re:" + data + "\nsw:" + result)
                    keyboard_page.exit.disconnect(receiver)
                }
                onTip: tip_page.setTip(outputText)
                onReminderTip: tip_page.setReminder(outputText)
                //                Connections {
                //                    target: page_Login.userLoginModule
                //                    onUserLoginCase: {

                //                    }
                //                }
            }
            states: [
                State {
                    name: "Show"
                    PropertyChanges {
                        target: login_page
                        y: login_page.y = mian_item.y
                        opacity: 1
                    }
                    PropertyChanges {
                        target: glass
                        fuzzy: true
                    }
                },
                State {
                    name: "default"
                    PropertyChanges {
                        target: login_page
                        y: parent.y - height
                        opacity: 0
                    }
                    PropertyChanges {
                        target: glass
                        fuzzy: false
                    }
                }
            ]
            transitions: [
                Transition {
                    to: "Show"
                    ParallelAnimation {
                        NumberAnimation {
                            target: login_page
                            properties: "y"
                            to: mian_item.y
                            duration: 300
                            easing.type: Easing.InOutExpo
                        }
                        NumberAnimation {
                            target: login_page
                            properties: "opacity"
                            to: 1
                            duration: 300
                            easing.type: Easing.OutSine
                        }
                    }
                },
                Transition {
                    to: "default"
                    ParallelAnimation {
                        NumberAnimation {
                            target: login_page
                            properties: "y"
                            to: mian_item.y - height
                            duration: 300
                            easing.type: Easing.InOutExpo
                        }
                        NumberAnimation {
                            target: login_page
                            properties: "opacity"
                            to: 0
                            duration: 300
                            easing.type: Easing.InCubic
                        }
                    }
                }
            ]
        }

        //页面状态
        states: [
            State {
                name: "GTerminal"
                PropertyChanges {
                    target: main_page
                    y: main_page.parent.y - main_page.parent.height
                }
                PropertyChanges {
                    target: glass
                    glasstraget: gTerminal_page
                }
            }
        ]
        //过度动画,模拟平推
        transitions: [
            Transition {
                to: "GTerminal"
                NumberAnimation {
                    target: main_page
                    properties: "y"
                    duration: 600
                    easing.type: Easing.InOutExpo
                }
            },
            Transition {
                to: "default"
                NumberAnimation {
                    target: main_page
                    properties: "y"
                    duration: 600
                    easing.type: Easing.InOutExpo
                }
            }
        ]
    }
}
