import QtQuick 2.9
import QtQuick.Controls 2.2
import "../pageqml"
import "../moduleqml"
import "../"

Item {
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
			width: parent.height * 3
			height: parent.height
			color: "transparent"
			radius: 5
			Label {
				anchors.centerIn: parent
				anchors.fill: parent
				//				color: "#ffffff"
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				font.pointSize: parent.height / 3 * 2
				text: "Terminal: "
			}
		}
		Rectangle {
			id: receive_area
			anchors.left: title_area.right
			width: parent.width - title_area.width - parent.height
			height: parent.height
			color: "transparent"
			radius: 5
			Label {
				anchors.centerIn: parent
				anchors.fill: parent
				//				color: "#ffffff"
				horizontalAlignment: Text.AlignLeft
				verticalAlignment: Text.AlignVCenter
				font.pointSize: parent.height / 3 * 2
				text: "<<< Receive < content"
			}
		}
		Rectangle {
			id: state_area
			anchors.left: receive_area.right
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
				gradient: Gradient {
					GradientStop {
						position: 0
						color: "#f83600"
					}

					GradientStop {
						position: 1
						color: "#f9d423"
					}
				}
			}
		}
	}
}
