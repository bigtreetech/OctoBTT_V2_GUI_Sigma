import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

Item {
    property string file_version: "1.0.0.0" //文件版本控制
    id: root
    //用法：
    //	root {
    //		id: hotend_area
    //		itemDirection: 0
    //		itemIndex: 0
    //		themeColor: "#ff0000"
    //		itemProportion: parent.itemsProportion
    //		limit_size: parent.limit_size
    //	}
    //	enum Direction {
    //		Left = 0,
    //		Right = 1,
    //		Top = 2,
    //		Bottom = 3
    //	}
    //按键依附方向 Left = 0 , Right = 1 , Top = 2 , Bottom = 3
    property int itemDirection: 0
    //按键位置索引
    property int itemIndex: 0
    //按键高度占比
    property double itemProportion: 0.25
    //主题色
    property color themeColor: "lightblue"
    //按键最大高度限制
    property int limit_size: 100
    //布局提升高度
    property int limit_z: 1
    property alias image: imagemodule.source /* 图片路径 */
    property alias text: textmodule.text /* 文本内容 */
    /*普通提醒*/
    signal tip(var outputText)
    /*强提醒*/
    signal reminderTip(var outputText)

    anchors.fill: parent
    //默认状态机为"默认"
    state: "default"
    //重新计算高度比
    function getItemProportion() {
        return itemsProportion * parent.height
                < limit_size ? itemsProportion : limit_size / parent.height
    }
    Rectangle //主要覆盖层
    {
        id: rectangle
        anchors.fill: parent
        color: "transparent"
        Rectangle {
            //Logo区域
            id: logoArea
            anchors.fill: edge_btn_area
            z: edge_btn_area.z + 1 + limit_z
            color: "transparent"
            opacity: root.state !== "default" ? 1 : textmodule.text.length > 0 ? 0.4 : 1
            anchors.margins: edge_btn_area.height * 0.2
            radius: edge_btn.radius
            clip: true
            Image {
                id: imagemodule
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
            }
            Behavior on opacity {
                NumberAnimation {
                    duration: 300
                }
            }
        }
        Rectangle {
            //文本区域
            id: textArea
            anchors.fill: edge_btn_area
            z: edge_btn_area.z + 2 + limit_z
            color: "transparent"
            opacity: root.state !== "default" ? 0 : 1
            anchors.margins: edge_btn_area.height * 0.25
            Behavior on opacity {
                NumberAnimation {
                    duration: 300
                }
            }
            Text {
                id: textmodule
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "white"
                anchors.fill: parent
                font.bold: true
                //				font.pointSize: text.length > 3 ? (parent.width / text.length
                //												   * 1.2) : (parent.width / 2)
                font.pointSize: getsize()
                function getsize() {
                    //自动计算字体大小
                    var pointsize = width / text.length
                    if (pointsize >= (height * 0.7))
                        pointsize = height * 0.7
                    pointsize = pointsize <= 0 ? 1 : pointsize
                    return pointsize
                }
            }
        }
        Rectangle //按钮域块
        {
            id: edge_btn_area
            anchors.left: itemDirection === 0 ? parent.left : undefined
            anchors.right: itemDirection === 1 ? parent.right : undefined
            height: parent.height * (getItemProportion(
                                         ) > 1 ? 1 : getItemProportion(
                                                     ) < 0 ? 0.25 : getItemProportion(
                                                                 ))
            //			anchors.topMargin: parent.height * 0.01
            width: height * 1.5
            //			color: themeColor
            color: "transparent"
            anchors.top: parent.top
            anchors.topMargin: height * itemIndex
            z: rectExpend.z + 1
            //			radius: 50
            //			anchors.verticalCenter: parent.verticalCenter
            //			clip: true
            Rectangle {
                id: edge_btn
                //				height: parent.height * 0.8
                //				width: parent.width - parent.height * 0.2
                width: edge_btn_area.width - edge_btn_area.height * 0.2
                anchors.left: itemDirection === 0 ? parent.left : undefined
                anchors.right: itemDirection === 1 ? parent.right : undefined
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: parent.height * 0.10
                color: themeColor
                radius: 200
                clip: true
                MouseArea {
                    hoverEnabled: true
                    anchors.fill: parent
                    onEntered: {
                        //防止动画冲突
                        enterAnim.running = root.state === "default"
                    }
                    onClicked: {
                        console.debug(parent, " : ", itemDirection, " : ",
                                      itemIndex)
                        //控制焦点转移
                        rectExpend.focus = true
                        //控制状态机
                        root.state = root.state === "default" ? "Click" : "default"
                    }
                }
            }
        }

        //工作空间
        Rectangle {
            id: rectExpend
            width: edge_btn.width / 2
            height: edge_btn.height
            radius: 20
            //跟随rectZoom设置
            color: root.themeColor
            y: edge_btn_area.anchors.topMargin + edge_btn_area.height * 0.1
            anchors.right: itemDirection === 1 ? parent.right : undefined
            anchors.left: itemDirection === 0 ? parent.left : undefined
            anchors.margins: width / 2

            //防止edge_btn遮挡
            z: 1

            //实际操作界面
            Rectangle {
                id: rectExpendContent
                width: parent.width * 0.95
                height: parent.height * 0.95
                anchors.centerIn: parent
                color: "transparent"
                clip: true
                opacity: 0
                radius: parent.radius

                MouseArea {
                    id: rectExpendContent_MouseArea
                    anchors.fill: parent
                    onClicked: {
                        console.debug(itemDirection + " : " + itemIndex)
                    }
                    property alias state: root.state

                    Connections {
                        target: windows
                        ignoreUnknownSignals: true //忽略未知信号
                        // @disable-check M16
                        onClosing: state = "default"
                    }
                    //绑定装载信号
                    Connections {
                        target: loader.item
                        ignoreUnknownSignals: true //忽略未知信号
                        // @disable-check M16
                        onReminderTip: root.reminderTip(outputText)
                        // @disable-check M16
                        onTip: root.tip(outputText)
                    }
                    //动态装载机器
                    Loader {
                        id: loader
                        anchors.fill: parent
                        source: {
                            switch (root.itemDirection) {
                            case 0:
                                switch (root.itemIndex) {
                                case 0:
                                    return "../pageqml/HotendModule.qml"
                                case 1:
                                    return "../pageqml/HeatBedModule.qml"
                                case 2:
                                    return "../pageqml/FanModule.qml"
                                case 3:
                                    return "../pageqml/ProgressModule.qml"
                                default:
                                    return undefined
                                }
                            case 1:
                                switch (root.itemIndex) {
                                case 0:
                                    return "../pageqml/SettingsModule.qml"
                                case 1:
                                    return "../pageqml/NetworkModule.qml"
                                case 2:
                                    return "../pageqml/PowerModule.qml"
                                case 3:
                                    return "../pageqml/CameraModule.qml"
                                default:
                                    return undefined
                                }
                            default:
                                return undefined
                            }
                        }
                    }
                }
            }
            onFocusChanged: {
                if (!rectExpend.focus)
                    root.state = "default"
            }
        }
    }

    //焦点进入按键动画 --- 缩放 回弹
    SequentialAnimation {
        id: enterAnim
        running: false
        loops: 1
        ParallelAnimation {
            NumberAnimation {
                target: edge_btn
                properties: "width"
                to: (edge_btn_area.width - edge_btn_area.height * 0.2) * 0.8
                duration: 150
                easing.type: Easing.OutBack
            }
        }

        ParallelAnimation {
            NumberAnimation {
                target: edge_btn
                properties: "width"
                to: edge_btn_area.width - edge_btn_area.height * 0.2
                duration: 150
                easing.type: Easing.OutBounce
            }
        }
    }

    onStateChanged: {
        enterAnim.running = false
    }

    //点击按键之后的按键机状态
    states: [
        State {
            name: "Click"
            //			PropertyChanges {
            //				target: textArea
            //				opacity: 0
            //			}
            //			PropertyChanges {
            //				target: logoArea
            //				opacity: 1
            //			}
            //按钮宽度延伸
            PropertyChanges {
                target: edge_btn
                width: root.width - edge_btn_area.width - edge_btn_area.height * 0.1
            }
            //最终体积位置
            PropertyChanges {
                target: rectExpend
                width: root.width - edge_btn_area.width * 2
                height: root.height - 2 * (edge_btn_area.height * 0.1)
                anchors.margins: edge_btn_area.width
                radius: 20
                y: edge_btn_area.height * 0.1
            }
            //提升item的z轴使其显示层提高
            PropertyChanges {
                target: edge_btn_area
                z: rectExpend.z - 1
            }
            PropertyChanges {
                target: root
                z: limit_z
            }
            //内容显示
            PropertyChanges {
                target: rectExpendContent
                opacity: 1
            }
        }
    ]

    //过度动画,模拟平推
    transitions: [
        Transition {
            from: "default"
            to: "Click"
            SequentialAnimation {
                //提升item的z轴使其显示层提高
                //				NumberAnimation {
                //					target: root
                //					properties: "z"
                //				}
                ParallelAnimation {
                    //按钮宽度延伸
                    NumberAnimation {
                        target: edge_btn
                        properties: "width"
                        duration: 200
                    }
                    //改变层高
                    NumberAnimation {
                        target: edge_btn_area
                        properties: "z"
                        duration: 200
                    }
                    //改变边距位置
                    NumberAnimation {
                        target: rectExpend
                        properties: "anchors.margins"
                        duration: 200
                    }
                    NumberAnimation {
                        target: rectExpend
                        properties: "width"
                        duration: 200
                    }
                }
                ParallelAnimation {
                    NumberAnimation {
                        target: rectExpend
                        properties: "height, y,radius"
                        duration: 300
                        easing.type: Easing.OutBounce
                    }
                }
                NumberAnimation {
                    target: rectExpendContent
                    properties: "opacity"
                    duration: 100
                }
            }
        },

        Transition {
            from: "Click"
            to: "default"
            SequentialAnimation {
                //				NumberAnimation {
                //					target: root
                //					properties: "z"
                //				}
                ParallelAnimation {
                    NumberAnimation {
                        target: rectExpendContent
                        properties: "opacity"
                        duration: 100
                    }
                    NumberAnimation {
                        target: rectExpend
                        properties: "height, y,radius"
                        duration: 100
                    }
                }

                ParallelAnimation {
                    NumberAnimation {
                        target: rectExpend
                        properties: "width"
                        duration: 100
                    }
                    //改变层高
                    NumberAnimation {
                        target: edge_btn_area
                        properties: "z"
                        duration: 100
                    }
                    //按钮宽度延伸
                    NumberAnimation {
                        target: edge_btn
                        properties: "width"
                        duration: 100
                    }
                    //改变边距位置
                    NumberAnimation {
                        target: rectExpend
                        properties: "anchors.margins"
                        duration: 100
                    }
                }
            }
        }
    ]
}
