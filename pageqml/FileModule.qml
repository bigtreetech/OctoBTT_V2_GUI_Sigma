import QtQuick 2.9
import "../pageqml"
import "../moduleqml"
import "../"

Item {
	id: parent_item
	/*普通提醒*/
	signal tip(var outputText)
	/*强提醒*/
	signal reminderTip(var outputText)
	property color borderColor
	property double borderWidth
	Rectangle {
		id: rect
		anchors.fill: parent
		color: "lightblue"
		border.color: borderColor
		border.width: borderWidth
	}
}
