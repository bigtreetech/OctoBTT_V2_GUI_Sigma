import QtQuick 2.9
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

Item {
	id: parentItem
	signal click_GTerminal
	signal loginfrm
	signal tip(var outputText)
	signal reminderTip(var outputText)
	signal sender(var data)
	anchors.fill: parent
	//		Glass {
	//			fuzzy: true
	//			traget: btnpanel_area
	//			positiontraget: btnpanel_area
	//		}
	//		Glass {
	//			fuzzy: true
	//			traget: logo_area
	//			positiontraget: logo_area
	//		}
	//		Glass {
	//			fuzzy: true
	//			traget: preview_area
	//			positiontraget: preview_area
	//		}
	//		Glass {
	//			fuzzy: true
	//			traget: gTerminal_area
	//			positiontraget: gTerminal_area
	//		}
	Rectangle {
		id: btnpanel_area
		width: parent.width
		anchors.top: logo_area.bottom
		anchors.bottom: gTerminal_area.top
		anchors.horizontalCenter: parent.horizontalCenter
		color: "transparent"
		z: 1
		Btnpanel_internal_center {
			id: btnpanel_area_child
			//			count: 7
			anchors.centerIn: parent
			anchors.fill: parent
			limit_size: windows.limit_size
			edgeProportion: windows.previewitemsProportion
			function send(data) {
				//					glasss.radius = glasss.radius == 64 ? 0 : 64
				console.log("Page_main_Receive: " + data)
			}
			onTip: parentItem.tip(outputText)
			onReminderTip: parentItem.reminderTip(outputText)
			Component.onCompleted: {
				/**信号处理器**/
				sender.connect(send)
			}
			onSender: parentItem.sender(data)
		}
	}

	Rectangle {
		id: logo_area
		height: parent.height / 6 <= 80 ? parent.height / 6 : 80
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		color: "transparent"
		z: 1
		Logo_center_top {
			id: logo_area_child
			anchors.centerIn: parent
			anchors.fill: parent
		}
	}
	Rectangle {
		id: preview_area
		anchors.top: logo_area.bottom
		anchors.bottom: gTerminal_area.top
		anchors.left: parent.left
		anchors.right: parent.right
		color: "transparent"
		z: 2
		Preview_PeripheralFunction {
			id: preview_area_child
			anchors.centerIn: parent
			anchors.fill: parent
			itemsProportion: previewitemsProportion
			limit_size: windows.limit_size
			onReminderTip: parentItem.reminderTip(outputText)
			onTip: parentItem.tip(outputText)
		}
	}
	Rectangle {
		id: gTerminal_area
		height: parent.height / 12 <= 40 ? parent.height / 12 : 40
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		color: "transparent"
		z: 1
		GTerminal_fill_bottom {
			id: gTerminal_area_child
			anchors.centerIn: parent
			anchors.fill: parent
			onClickpanel: click_GTerminal()
			onClickstate: {
				console.debug("gTerminal_area_child stateclick")
				loginfrm()
			}
			onReminderTip: parentItem.reminderTip(outputText)
			onTip: parentItem.tip(outputText)
		}
	}
}

/*##^##
Designer {
	D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

