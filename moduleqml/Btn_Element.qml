import QtQuick 2.9
import "../pageqml"
import "../moduleqml"
import "../"

Item {
	//	width: 560
	//	height: 360
	id: motherItem
	property Item parentTarget: elementTarget
	property Item elementTarget: parent
	property double parentX: elementX
	property double elementX
	property double parentY: elementY
	property double elementY
	property double parentWidth: elementWidth
	property double elementWidth
	property double parentHeight: elementHeight
	property double elementHeight
	property int elementId: -1 //键位id
	property color color
	property point positionOffset: Qt.point(0, 0)
	property double borderOffset: 0
	property double heightOffset: 0
	/*普通提醒*/
	signal tip(var outputText)
	/*强提醒*/
	signal reminderTip(var outputText)
	state: "default"
	x: elementTarget.x
	y: elementTarget.y
	width: elementTarget.width
	height: elementTarget.height
	z: state === "default" ? 0 : 1
	onFocusChanged: {
		if (focus === false)
			motherItem.state = "default"
	}
	signal clicked(var listid)
	///function
	function getBtn(type) {
		//获取按键名称
		switch (motherItem.elementId) {
		case 0:
			return type === "Text" ? qsTr("File") : type === "Img" ? "qrc:/assets/wenjian.svg" : ""
		case 1:
			return type === "Text" ? qsTr("Bed Level") : type
									 === "Img" ? "qrc:/assets/kongzhimianban.svg" : ""
		case 2:
			return type === "Text" ? qsTr("LED") : type === "Img" ? "" : ""
		case 3:
			return type === "Text" ? qsTr("Home") : type
									 === "Img" ? "qrc:/assets/huidaoyuandian.svg" : ""
		case 4:
			return type === "Text" ? qsTr("Cool") : type
									 === "Img" ? "qrc:/assets/shuangdong.svg" : ""
		case 5:
			return type === "Text" ? qsTr("Unlock") : type === "Img" ? "qrc:/assets/jiesuo.svg" : ""
		case 6:
			return type === "Text" ? qsTr("AC") : type
									 === "Img" ? "qrc:/assets/yonghukongzhitaiicon.svg" : ""
		case 7:
			return type === "Text" ? qsTr("Custom") : type
									 === "Img" ? "qrc:/assets/chuizhijuzhong.svg" : ""
		default:
			return type === "Text" ? qsTr("Unknow") : type === "Img" ? "" : ""
		}
	}
	Rectangle {
		anchors.fill: parent
		color: "transparent"
		///Layout Area
		Rectangle {
			///theme style
			anchors.fill: parent
			color: "transparent"
			AllBtnStyle {
				id: btn
				btnTextContent: motherItem.getBtn("Text")
				btnImagePath: motherItem.getBtn("Img")
				anchors.fill: parent
				btnColor: motherItem.color
				onKeySignal: clicked(elementId)
			}
		}
	}
	///States Group
	states: []
	///Animation
	Behavior on x {
		NumberAnimation {
			duration: 150
		}
	}
	Behavior on y {
		NumberAnimation {
			duration: 150
		}
	}
	Behavior on width {
		NumberAnimation {
			duration: 150
		}
	}
	Behavior on height {
		NumberAnimation {
			duration: 150
		}
	}
}

/*##^##
Designer {
	D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

