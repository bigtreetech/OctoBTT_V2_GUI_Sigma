import QtQuick 2.9
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

Item {
    id: root
    property string file_version: "1.0.0.0" //文件版本控制
    width: 300
    height: 50
    /*普通提醒*/
    signal tip(var outputText)
    /*强提醒*/
    signal reminderTip(var outputText)
    signal clicked
    property color checkedColor: "#0ACF97" //滑块选择后的底色
    property color uncheckedColor: "#E5E5E5" //滑块反选后的底色
    property color uncheckedTextColor: uncheckedColor //文字反选后的底色
    property color checkedTextColor: checkedColor //文字选择后的底色
    property bool checked: false //滑块选择标记
    property string text: "Test" //显示文本
    property int radius: 10 //外框圆角
    property int animationTime: 250 //动画运行时间
    property double contantWidth: textLayer.contentWidth + accordingLayer.height
                                  * 0.4 + btn.width + 2
    //操控层
    MouseArea {
        id: cursorArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        clip: true
        hoverEnabled: true
        onClicked: {
            root.checked = !root.checked
            root.clicked()
        }
        onEntered: {
            accordingLayer.border.color = root.checkedColor
            accordingLayer.border.width = 2
            textLayer.color = root.checkedTextColor
        }
        onExited: {
            accordingLayer.border.color = Qt.binding(function () {
                return root.checked ? root.checkedColor : root.uncheckedColor
            })
            textLayer.color = Qt.binding(function () {
                return root.checked ? root.checkedTextColor : root.uncheckedTextColor
            })
            accordingLayer.border.width = Qt.binding(function () {
                return root.checked ? 2 : 1
            })
        }
    }
    //蒙板遮蔽层
    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        color: "#555555" //蒙板颜色
        clip: true
        radius: root.radius
        opacity: root.enabled ? 0 : 0.3
        z: 1
        Behavior on opacity {
            NumberAnimation {
                duration: root.animationTime
            }
        }
    }
    //显示层
    Rectangle {
        id: accordingLayer
        anchors.fill: parent
        anchors.margins: 2
        color: "transparent"
        clip: true
        radius: root.radius
        border.color: root.checked ? root.checkedColor : root.uncheckedColor //图层边框色彩
        border.width: root.checked ? 2 : 1 //图层遮蔽线粗
        Behavior on border.color {
            ColorAnimation {
                duration: root.animationTime
            }
        }
        Behavior on border.width {
            NumberAnimation {
                duration: root.animationTime
            }
        }
        ///开关滑块
        Rectangle {
            id: btn
            color: "transparent"

            anchors.topMargin: parent.height * 0.1
            anchors.bottomMargin: parent.height * 0.1
            anchors.leftMargin: parent.height * 0.1

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            width: height * 1.75
            //滑块显示底层面板
            Rectangle {
                anchors.fill: parent
                color: root.checked ? root.checkedColor : root.uncheckedColor
                radius: height / 2
                border.color: root.checked ? root.checkedColor : "#E5E5E5"
                Behavior on border.color {
                    ColorAnimation {
                        duration: root.animationTime
                    }
                }
                Behavior on color {
                    ColorAnimation {
                        duration: root.animationTime
                    }
                }
                //滑块圆钮
                Rectangle {
                    x: root.checked ? parent.width - (parent.height - 6) - 3 : 1
                    width: root.checked ? parent.height - 6 : parent.height - 2
                    height: width
                    radius: width / 2
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    border.color: "#D5D5D5"

                    Behavior on x {
                        //平移动画
                        NumberAnimation {
                            duration: root.animationTime
                            easing.type: Easing.OutBack //动画策略
                        }
                    }
                    Behavior on width {
                        //缩放同步动画
                        NumberAnimation {
                            duration: root.animationTime
                            easing.type: Easing.InExpo
                        }
                    }
                }
            }
        }
        ///文本区域
        Rectangle {
            anchors.left: btn.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            anchors.margins: parent.height * 0.1
            clip: true

            color: "transparent"
            Text {
                id: textLayer
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: root.text
                // font.bold:root.checked
                font.pointSize: height / 3 * 2 //字体大小
                color: root.checked ? root.checkedTextColor : root.uncheckedTextColor //字体颜色控制
                Behavior on color {
                    //字体色彩渐变动画
                    ColorAnimation {
                        duration: root.animationTime
                    }
                }
            }
        }
    }
}
