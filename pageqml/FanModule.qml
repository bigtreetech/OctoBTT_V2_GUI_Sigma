import QtQuick 2.9
import "../pageqml"
import "../moduleqml"
import "../"

Item {
	id: parentItem
	/*普通提醒*/
	signal tip(var outputText)
	/*强提醒*/
	signal reminderTip(var outputText)
	Text {
		text: "123"
	}
	Rectangle {
		anchors.fill: parent
		color: "transparent"
		border.width: 1
		MouseArea {
			anchors.fill: parent
			onClicked: parentItem.reminderTip("var outputText")
		}
	}
}
