import QtQuick 2.9
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

//import MyCameraModule 1.0
Item {
    property string file_version: "1.0.0.0" //文件版本控制
    id: root
    /*普通提醒*/
    signal tip(var outputText)
    /*强提醒*/
    signal reminderTip(var outputText)
    property color borderColor
    property double borderWidth
    property bool cameraRun: root.parent.parent.state !== "default"
    property var settings
    property bool imageInvalid: true
    Connections {
        target: functionModule
        // @disable-check M16
        onSettingsUpdated: {
            root.getsetting(json)
        }
    }
    function getsetting(jsonpacket) {
        root.settings = JSON.parse(jsonpacket)
        flipH_Btn.checked = Qt.binding(function () {
            return root.settings.webcam.flipH
        })
        flipV_Btn.checked = Qt.binding(function () {
            return root.settings.webcam.flipV
        })
        flip90_Btn.checked = Qt.binding(function () {
            return root.settings.webcam.rotate90
        })
    }
    onCameraRunChanged: {
        if (root.cameraRun) {
            preload.source = preload.address + preload.path + Date.now()
        } else {
            root.imageInvalid = true
        }
    }
    Component.onDestruction: {
        myCameraModule.closeCamera()
    }
    //    MyCameraModule {
    //        id: myCameraModule
    //        address: "http://" + globalAccess.mainIP + "/"
    //        onSentQImage: {
    //            //            viewer.update()
    //            //            viewer.data = image
    //            //            viewer.source = ""
    //            //            if (globalAccess.userinfo.login)
    //            //                viewer.source = "image://camera/" + Date.now()
    //            //            else
    //            //                viewer.source = ""
    //            //            viewer.source = "http://192.168.0.41/webcam/?action=snapshot" + Date.now()
    //            root.imageInvalid = false
    //        }
    //        Component.onCompleted: {
    //            if (root.cameraRun)
    //                myCameraModule.openCamera()
    //            else
    //                myCameraModule.closeCamera()
    //        }
    //    }
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "transparent"
        border.color: borderColor
        border.width: borderWidth
        clip: true
        radius: 50
        Rectangle {
            id: viewerable
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.85
            rotation: viewer.flip90 ? -90 : 0
            color: "transparent"
            clip: true
            radius: parent.radius
            Behavior on rotation {
                NumberAnimation {
                    duration: 250
                }
            }
            scale: viewer.flip90 ? viewerable.height / viewerable.width : 1
            Behavior on scale {
                NumberAnimation {
                    duration: 100
                }
            }
            Image {
                id: preload
                anchors.fill: parent
                visible: false
                asynchronous: true
                fillMode: Image.PreserveAspectFit
                //                    source: address + path + Date.now()
                antialiasing: true
                property string address: "http://" + globalAccess.mainIP + "/"
                property string path: "webcam/?action=snapshot&"
                onStatusChanged: {
                    if (status === Image.Ready) {
                        preload.grabToImage(function (result) {
                            if (root.cameraRun && globalAccess.userinfo.login) {
                                viewer.source = result.url
                                root.imageInvalid = false
                                preload.source = preload.address + preload.path + Date.now()
                            } else {
                                viewer.source = ""
                            }
                        }, Qt.size(preload.width * 2, preload.height * 2))
                    }
                }
            }
            Rectangle {
                id: viewer_flipH
                anchors.fill: parent
                color: "transparent"
                transform: Rotation {
                    origin.x: viewer.width / 2
                    origin.y: viewer.height / 2
                    axis.x: 0
                    axis.y: 1
                    axis.z: 0
                    angle: viewer.flipH ? 180 : 0
                    Behavior on angle {
                        NumberAnimation {
                            duration: 250
                        }
                    }
                }
                Image {
                    id: viewer
                    anchors.fill: parent
                    property bool flipH: root.settings.webcam.flipH
                    property bool flipV: root.settings.webcam.flipV
                    property bool flip90: root.settings.webcam.rotate90
                    fillMode: Image.PreserveAspectFit
                    antialiasing: true
                    cache: false
                    asynchronous: true
                    transform: Rotation {
                        origin.x: viewer.width / 2
                        origin.y: viewer.height / 2
                        axis.x: 1
                        axis.y: 0
                        axis.z: 0
                        angle: viewer.flipV ? 180 : 0
                        Behavior on angle {
                            NumberAnimation {
                                duration: 250
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            id: filp_area
            anchors.top: viewerable.bottom
            //            height: parent.height / 5
            z: 2
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            color: "transparent"
            FlatSwitch {
                id: flipH_Btn
                enabled: globalAccess.userinfo.login && !root.imageInvalid
                anchors.margins: 2
                checkedColor: "#8efc60"
                text: qsTr("H")
                radius: 10
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 3 - anchors.margins * 6
                checked: root.settings.webcam.flipH
                onClicked: {
                    //                    viewer.flipH = checked
                    var settingPathList = ["webcam", "flipH"]
                    var settingPathIsArray = [false, false]
                    var settingValue = checked
                    functionModule.postSetting(settingPathList,
                                               settingPathIsArray, settingValue)
                }
            }

            FlatSwitch {
                id: flipV_Btn
                enabled: globalAccess.userinfo.login && !root.imageInvalid
                anchors.margins: 2
                checkedColor: "#8efc60"
                text: qsTr("V")
                radius: 10
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 3 - anchors.margins * 6
                checked: root.settings.webcam.flipV
                onClicked: {
                    //                    viewer.flipV = checked
                    var settingPathList = ["webcam", "flipV"]
                    var settingPathIsArray = [false, false]
                    var settingValue = checked
                    functionModule.postSetting(settingPathList,
                                               settingPathIsArray, settingValue)
                }
            }

            FlatSwitch {
                id: flip90_Btn
                enabled: globalAccess.userinfo.login && !root.imageInvalid
                anchors.margins: 2
                checkedColor: "#8efc60"
                text: qsTr("90'")
                radius: 10
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 3 - anchors.margins * 6
                checked: root.settings.webcam.rotate90
                onClicked: {
                    //                    viewer.flip90 = checked
                    var settingPathList = ["webcam", "rotate90"]
                    var settingPathIsArray = [false, false]
                    var settingValue = checked
                    functionModule.postSetting(settingPathList,
                                               settingPathIsArray, settingValue)
                }
            }
        }
    }
}
