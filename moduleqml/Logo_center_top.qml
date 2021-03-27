import QtQuick 2.9
import QtQuick.Controls 2.2
import "../pageqml"
import "../moduleqml"
import "../"

Item {
	width: 600
	height: 80
	Rectangle {
		anchors.centerIn: parent
		height: parent.height * 0.9
		width: parent.width * 0.9
		color: "transparent"
		clip: true
		Rectangle {
			id: logoname_area
			height: parent.height
			width: parent.width / 5 * 2 - 1
			color: "transparent"
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			Image {
				id: logoname
				anchors.right: parent.right
				width: parent.height * 0.8
				height: parent.height * 0.8
				anchors.verticalCenter: parent.verticalCenter
				source: "qrc:/assets/icon/icon-main-btt-blue.svg"
				fillMode: Image.PreserveAspectFit
			}
		}
		Rectangle {
			id: idname_area
			height: parent.height
			width: parent.width / 5 * 3
			color: "transparent"
			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
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
				function getsize() {
					//自动计算字体大小
					var pointsize = width / text.length
					if (pointsize >= height)
						pointsize = height * 0.9
					pointsize = pointsize <= 0 ? 1 : pointsize
					return pointsize
				}
				anchors.verticalCenter: parent.verticalCenter
				anchors.left: parent.left
			}
		}
	}
}
