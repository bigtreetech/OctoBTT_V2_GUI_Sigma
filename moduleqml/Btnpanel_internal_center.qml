import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

Item {
    property string file_version: "1.0.0.0" //文件版本控制
    id: root
    width: 800
    height: 480
    property color basecolor: "gray"
    property int count: 9 //生产元素数量
    property int limit_line: 2 //限制元素行数
    property int limit_size: 100 //元素限制大小
    property double edgeProportion: 0.25 //边缘元素垂直比例
    /*普通提醒*/
    signal tip(var outputText)
    /*强提醒*/
    signal reminderTip(var outputText)
    signal sender(var data)
    function accumulationAlgorithm() {
        var initsize = gridView.height < limit_size
                * limit_line ? gridView.height / limit_line : gridView.height / Math.floor(
                                   gridView.height / limit_size)
        if (Math.ceil(count / Math.floor(
                          gridView.height / initsize)) >= gridView.width)
            return Number(initsize) //返回大小
        else {
            for (var i = Math.floor(
                     gridView.height / initsize); i > limit_line; --i) {
                var setsize = gridView.height / i //单元大小
                var horizontalLenght = Math.ceil(count / i) //水平个数
                if (horizontalLenght * setsize >= gridView.width)
                    return Number(setsize) //返回大小
            }
            return Number(gridView.height / limit_line) //返回大小
        }
    }

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

    //WorkScript
    function doEveryThing(elementId) {
        //执行响应功能
        console.log("doEveryThing: " + elementId)
        switch (elementId) {
        case 0:
            //File
            workArea.setState("case" + elementId)
            return
        case 1:
            //Bed Level
            functionModule.sentGcodeCommand(
                        ["M502", "M500", "G91", "G28", "G29", "G90", "M500"])
            return
        case 2:
            //LED
            workArea.setState("case" + elementId)
            return
        case 3:
            //Home
            functionModule.sentGcodeCommand(["G91", "G28", "G90"])
            break
        case 4:
            //Cool
            functionModule.sentGcodeCommand(["M106 S255", "M104 S0", "M140 S0"])
            break
        case 5:
            //Unlock
            functionModule.sentGcodeCommand("M18")
            break
        case 6:
            //AC
            sender("ACLogin")
            return
        case 7:
            //Auto Connect
            functionModule.postConnect(
                        root.currentInfo !== undefined
                        && root.currentInfo.current.state.flags.operational
                        === true ? "disconnect" : "connect")
            return
        case 8:
            //MachineConfiguration
            workArea.setState("case" + elementId)
            return
        default:
            //Unknow
            workArea.setState("Expend")
            break
        }
        sender(elementId)
    }
    //WorkArea
    Rectangle {
        id: workArea
        anchors.fill: rectArea
        border.color: rectArea.border.color
        border.width: rectArea.border.width
        color: "transparent"
        enabled: false
        opacity: enabled ? 1 : 0
        z: glass.z + 1
        focus: false
        state: "default"
        property int elementId: -1
        function setState(stateName) {
            focus = true
            if (stateName.indexOf("case") >= 0) {
                workArea.state = "case"
                elementId = parseInt(stateName.replace("case", ""))
            } else
                workArea.state = stateName
        }
        onFocusChanged: {
            console.debug("workArea:" + focus)
            if (!focus && !loader.focus) {
                state = "default"
                elementId = -1
            }
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                parent.focus = true
            }
            onWheel: {

            }
            //绑定装载信号
            Connections {
                target: loader.item
                ignoreUnknownSignals: true //忽略未知信号
                // @disable-check M16
                onReminderTip: preview_item.reminderTip(outputText)
                // @disable-check M16
                onTip: preview_item.tip(outputText)
            }
            Loader {
                id: loader
                anchors.fill: parent
                source: {
                    switch (workArea.elementId) {
                    case 0:
                        return "../pageqml/FileModule.qml"
                    case 1:
                        return "../pageqml/BedLevelModule.qml"
                    case 2:
                        return "../pageqml/LEDModule.qml"
                    case 8:
                        return "../pageqml/MachineConfiguration.qml"
                    default:
                        return ""
                    }
                }
                onLoaded: {
                    loader.item.borderColor = workArea.border.color
                    loader.item.borderWidth = workArea.border.width
                }
            }
        }
        ///状态组
        states: [
            State {
                //展开测试
                name: "Expend"
                PropertyChanges {
                    target: workArea
                    color: "transparent"
                    enabled: true
                }
                PropertyChanges {
                    target: glass
                    fuzzy: true
                }
            },
            State {
                name: "case"
                PropertyChanges {
                    target: workArea
                    opacity: 1
                    enabled: true
                }
                PropertyChanges {
                    target: glass
                    fuzzy: true
                }
            },
            State {
                name: "default"
                PropertyChanges {
                    target: workArea
                    color: "transparent"
                    enabled: false
                }
                PropertyChanges {
                    target: glass
                    fuzzy: false
                }
            }
        ]

        ///动画组
        Behavior on opacity {
            NumberAnimation {
                duration: 500
            }
        }
        Behavior on color {
            ColorAnimation {
                duration: 500
            }
        }
    }
    Glass {
        z: rectArea.z + 1
        id: glass
        fuzzy: false
        glasstraget: rectArea
    }
    //CellArea
    Rectangle {
        id: rectArea
        height: parent.height * 0.9
        width: (parent.width - (edgeProportion * parent.height
                                < limit_size ? edgeProportion * parent.height
                                               * 1.5 : limit_size * 1.5) * 2) * 0.9
        anchors.centerIn: parent
        color: "#4d808080"
        border.color: "green"
        border.width: 4
        clip: true
        z: 1
        Rectangle {
            id: dataArea
            height: parent.height * 0.9
            width: parent.width - parent.border.width * 2
            anchors.centerIn: parent
            color: "transparent"
            GridView {
                id: gridView
                anchors.fill: parent
                cellHeight: accumulationAlgorithm()
                cellWidth: cellHeight
                flow: Grid.TopToBottom
                interactive: false
                contentX: listView.contentX
                model: gridModel
                delegate: gridDel
                clip: true
                z: 1
            }
            ListView {
                id: listView
                anchors.fill: parent
                model: listModel
                delegate: listDel
                orientation: ListView.Horizontal
                //				snapMode: ListView.SnapPosition
                clip: true
                z: 0

                //				//低层事件接收测试
                //				MouseArea {
                //					anchors.fill: parent
                //					onClicked: {
                //						mouse.accepted = false
                //						console.debug("check")
                //					}
                //				}
            }
            Component {
                id: gridDel
                Rectangle {
                    height: gridView.cellHeight
                    width: height
                    color: "transparent"
                    z: chirldItem.z
                    //					border.color: "black"
                    Rectangle {
                        id: rect
                        anchors.centerIn: parent
                        width: parent.width * 0.7
                        height: parent.height * 0.7
                        radius: 2
                        //					border.color: "black"
                        color: "transparent"
                        Btn_Element {
                            id: chirldItem
                            parentTarget: parent.parent.parent.parent.parent.parent.parent
                            anchors.fill: parent
                            borderOffset: rectArea.border.width
                            positionOffset: Qt.point(
                                                (parent.parent.width - parent.width) / 2,
                                                (parent.parent.height - parent.height) / 2
                                                + (rectArea.height - dataArea.height)
                                                / 2 - borderOffset)
                            heightOffset: rectArea.height - dataArea.height
                            elementId: input_id
                            color: basecolor
                            onClicked: doEveryThing(listid)
                        }
                    }
                }
            }
            ListModel {
                id: gridModel
            }
            Component {
                id: listDel
                Rectangle {
                    //										width: gridView.width
                    width: Math.ceil(
                               count / Math.floor(
                                   gridView.height / accumulationAlgorithm(
                                       ))) * accumulationAlgorithm()
                    height: gridView.height
                    color: "transparent"
                    //					border.color: "black"
                }
            }
            ListModel {
                id: listModel
            }
            Component.onCompleted: {
                for (var i = 0; i < count; ++i) {
                    gridModel.append({
                                         "input_id": i
                                     })
                }
                listModel.append({})
                //				for (var j = 0; j < Math.ceil(
                //						 count / Math.floor(
                //							 gridView.height / accumulationAlgorithm())); ++j) {
                //					listModel.append({})
                //				}
            }
        }
    }
}
