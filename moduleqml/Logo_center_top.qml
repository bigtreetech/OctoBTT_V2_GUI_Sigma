import QtQml 2.0
import QtQuick 2.9
import QtQuick.Controls 2.2
import "../pageqml"
import "../moduleqml"
import "../"

Item {
    property string file_version: "1.0.0.0" //文件版本控制
    width: 600
    height: 80
    id: root
    Rectangle {
        anchors.centerIn: parent
        height: parent.height * 0.9
        width: logoname_area.width + idname.contentWidth
        color: "transparent"
        clip: true
        Rectangle {
            id: logoname_area
            height: parent.height
            width: height
            color: "transparent"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            Image {
                id: logoname
                anchors.fill: parent
                anchors.margins: parent.height * 0.1
                //                anchors.right: parent.right
                //                width: parent.height * 0.8
                //                height: parent.height * 0.8
                //                anchors.verticalCenter: parent.verticalCenter
                source: "../assets/icon/icon-main-btt-blue.svg"
                fillMode: Image.PreserveAspectFit
            }
        }
        Rectangle {
            id: idname_area
            height: parent.height
            //            width: parent.width / 5 * 3
            color: "transparent"
            anchors.left: logoname_area.right
            anchors.verticalCenter: parent.verticalCenter
            //            anchors.right: parent.right
            width: idname.contentWidth
            Label {
                id: idname
                height: parent.height
                color: "#00aaff"
                text: qsTr("OctoBTT")
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.bold: true
                //				font.pixelSize: parent.height / 3 * 2
                font.pointSize: getsize()
                width: contentWidth
                function getsize() {
                    //自动计算字体大小
                    var pointsize = height
                    if (pointsize >= (height * 0.5))
                        pointsize = height * 0.5
                    pointsize = pointsize <= 0 ? 1 : pointsize
                    return pointsize
                }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
            }
        }
    }
}
