import QtQuick 2.9
import QtQml 2.0
import QtGraphicalEffects 1.0
import "../pageqml"
import "../moduleqml"
import "../"

//import QtQml 2.0
Item {
    property string file_version: "1.0.0.0" //文件版本控制
    //用法：
    //	Glass {
    //		fuzzy: true
    //		glasstraget: gTerminal_area
    //		positiontraget: btnpanel_area
    //	}
    property Item glasstraget
    property Item positiontraget: glasstraget
    property bool fuzzy: false
    anchors.fill: glasstraget
    z: positiontraget.z + 1
    id: root
    //毛玻璃
    Rectangle {
        height: parent.height
        width: parent.width
        anchors.fill: parent
        color: "transparent"
        anchors.centerIn: parent

        FastBlur {
            id: glasss
            anchors.fill: parent
            //            transparentBorder: true
            source: glasstraget
            radius: fuzzy ? 48 : 0 //max 144
            Behavior on radius {
                NumberAnimation {
                    duration: 500
                }
            }
            //			Timer {
            //				running: fuzzy
            //				repeat: true
            //				interval: 500
            //				onTriggered: {
            //					glasss.radius = fuzzy ? glasss.radius === 48 ? 0 : 48 : 0
            //				}
            //				onRunningChanged: {
            //					glasss.radius = fuzzy ? 48 : 0
            //				}
            //			}
        }
        Rectangle {
            ///毛玻璃底色
            color: "lightgray"
            opacity: fuzzy ? 0.3 : 0
            anchors.fill: parent
            z: 1
            Behavior on opacity {
                NumberAnimation {
                    duration: 1000
                }
            }
        }
    }
}
