import QtQuick 2.9
import QtQml 2.0
import QtQuick.Controls 2.2
import "../pageqml"
import "../moduleqml"
import "../"

Item {
    property string file_version: "1.0.0.0" //文件版本控制
    id: root
    width: 480
    height: 40
    signal clickpanel
    signal clickstate
    /*普通提醒*/
    signal tip(var outputText)
    /*强提醒*/
    signal reminderTip(var outputText)
    Rectangle {
        anchors.bottom: main_area.bottom
        color: main_area.color
        height: main_area.height / 2
        width: main_area.width
    }
    Rectangle {
        id: main_area
        anchors.centerIn: parent
        anchors.fill: parent
        color: "gray"
        radius: 200
        clip: true
        MouseArea {
            anchors.fill: parent
            onClicked: {
                clickpanel()
            }
        }
        MouseArea {
            anchors.fill: state_area
            z: 1
            onClicked: {
                console.debug("click state")
                clickstate()
            }
        }
        Rectangle {
            id: title_area
            anchors.left: parent.left
            width: title.contentWidth
            height: parent.height
            color: "transparent"
            radius: 5
            anchors.leftMargin: height / 2
            Label {
                id: title
                anchors.centerIn: parent
                anchors.fill: parent
                //				color: "#ffffff"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: (parent.height <= 0 ? 1 : parent.height) * 0.5
                text: "Terminal: "
            }
        }
        Rectangle {
            id: state_area
            //            anchors.left: receive_area.right
            anchors.right: parent.right
            width: parent.height
            height: parent.height
            color: "transparent"
            radius: 5
            Rectangle {
                width: parent.height * 0.8
                height: parent.height * 0.8
                radius: 50
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                //                color: "red"
                //                Behavior on color {
                //                    ColorAnimation {
                //                        duration: 1000
                //                    }
                //                }
                gradient: Gradient {
                    GradientStop {
                        id: lightstart
                        position: 0
                        color: "red"
                        Behavior on color {
                            ColorAnimation {
                                duration: 600
                            }
                        }
                    }

                    GradientStop {
                        id: lightstop
                        position: 1
                        color: "yellow"
                        Behavior on color {
                            ColorAnimation {
                                duration: 1200
                            }
                        }
                    }
                }
                Connections {
                    target: page_Login.userLoginModule
                    onIpCase: {
                        if (online) {
                            lightstart.color = "orange"
                            lightstop.color = "gold"
                        } else {
                            lightstart.color = "red"
                            lightstop.color = "orange"
                        }
                    }
                    onUserLoginCase: {
                        console.debug(loginState)
                        switch (loginState) {
                        case 0x000:
                            //0x000:需要登陆
                            //0x1000:IP地址可用
                            lightstart.color = "blue"
                            lightstop.color = "yellow"
                            break
                        case 0x100:
                        case 0x111:
                            //0x100:免注册登录直接使用
                            //0x111:登陆成功
                            lightstart.color = "lime"
                            lightstop.color = "yellow"
                            break
                        default:
                            lightstart.color = "orange"
                            lightstop.color = "red"
                            break
                        }
                    }
                }
            }
        }
        Rectangle {
            id: receive_area
            anchors.left: title_area.right
            anchors.right: state_area.left
            //            width: parent.width - title_area.width - parent.height - title_area.anchors.leftMargin
            height: parent.height
            color: "transparent"
            radius: 5
            clip: true
            Label {
                id: receiver
                anchors.centerIn: parent
                anchors.fill: parent
                //				color: "#ffffff"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pointSize: (parent.height <= 0 ? 1 : parent.height) * 0.5
                //                text: ""
                Connections {
                    target: myWebsoket
                    onSentSearchLinkMsg: {
                        receiver.text = msg
                    }
                }
            }
        }
    }
}
