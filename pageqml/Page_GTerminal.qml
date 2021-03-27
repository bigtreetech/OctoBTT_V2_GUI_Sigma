import QtQuick 2.9
import QtQuick.Controls 2.2
import "../pageqml"
import "../moduleqml"
import "../"

Item {
	anchors.fill: parent
	//返回事件
	signal click_back
	//键盘事件
	signal click_keyboard(var inputdata)
	//发送事件
	signal click_send(var data)
	//发送事件
	signal text_update
	//接收消息控件
	property alias receive_data: receive_area.text
	//键盘输入事件
	signal setText(var inputdata, var result)
	/*普通提醒*/
	signal tip(var outputText)
	/*强提醒*/
	signal reminderTip(var outputText)
	Rectangle {
		height: parent.height - back_area.height
		width: parent.width
		anchors.bottom: parent.bottom
		clip: true
		anchors.horizontalCenter: parent.horizontalCenter
		color: "transparent"
		Rectangle {
			id: input_area
			height: parent.height * 0.2 > 100 ? 100 : parent.height * 0.2
												< 50 ? 50 : parent.height * 0.2
			width: parent.width
			anchors.bottom: parent.bottom
			clip: true
			anchors.horizontalCenter: parent.horizontalCenter
			color: "transparent"
			Rectangle {
				id: rectangle
				anchors.fill: parent
				//				anchors.margins: 1
				color: "transparent"
				//				border.color: "darkgray"
				radius: 10
				///文本输入区域
				Rectangle {
					height: parent.height
					anchors.verticalCenter: parent.verticalCenter
					width: parent.width - btn_send_area.width
					anchors.left: parent.left
					id: text_area
					color: "transparent"
					clip: true

					Rectangle {
						anchors.fill: parent
						anchors.margins: 2
						clip: true
						color: "transparent"
						radius: 10
						border.color: "darkgray"
						Label {
							id: code_area
							anchors.fill: parent
							horizontalAlignment: Text.AlignLeft
							verticalAlignment: Text.AlignVCenter
							clip: true
							font.pointSize: parent.height / 2
							color: "#FFFFFF"
							///引导键盘事件区域
							MouseArea {
								anchors.fill: parent
								onClicked: {
									console.debug(parent.text)
									setText.connect(setting)
									click_keyboard(parent.text)
								}
								function setting(data, result) {
									if (result) {
										parent.text = data
									}
									setText.disconnect(setting)
								}
							}
						}
					}
				}
				///发送按钮区域
				Rectangle {
					id: btn_send_area
					height: parent.height
					width: height * 2
					color: "transparent"
					anchors.verticalCenter: parent.verticalCenter
					anchors.right: parent.right
					AllBtnStyle {
						id: btn
						btnTextContent: qsTr("Send")
						anchors.fill: parent
						anchors.margins: 2
						btnColor: "#008800"
						btnTextFontColor: "#00FF00"
						onKeySignal: click_send(code_area.text)
						radius: 200
					}
				}
			}
		}
		Rectangle {
			id: disp_area
			height: parent.height - input_area.height
			width: parent.width
			anchors.bottom: input_area.top
			clip: true
			anchors.horizontalCenter: parent.horizontalCenter
			color: "transparent"
			//			border.color: "black"
			///显示消息区域
			ScrollView {
				id: scroll_area
				anchors.fill: parent
				anchors.margins: 1
				clip: true
				ScrollBar.vertical.policy: ScrollBar.AlwaysOn
				ScrollBar.vertical.interactive: true
				ScrollBar.vertical.width: 30
				background: Rectangle {
					anchors.fill: parent
					color: "transparent"
					border.color: "darkgray"
					radius: 5
				}
				contentWidth: disp_area.width - 2 * anchors.margins - ScrollBar.vertical.width
				TextEdit {
					width: parent.width
					anchors.bottom: parent.bottom
					anchors.margins: 1
					id: receive_area
					wrapMode: TextEdit.WrapAnywhere
					readOnly: true
					font.pointSize: input_area.height / 3
					color: "lime"
					//					text: "import QtQuick.Controls 0.2 import QtQuick.Controls 1.2 import QtQuick.Controls 2.2 import QtQuick.Controls 3.2 import QtQuick.Controls 4.2\n\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2 import QtQuick.Controls 2.2 import QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2\nimport QtQuick.Controls 2.2"
					onTextChanged: {
						text_update()
						console.debug(scroll_area.contentItem.width)
						console.debug(width)
						console.debug(scroll_area.width)
						scroll_area.contentItem.contentY = scroll_area.contentHeight
								- scroll_area.contentItem.height
					}
					///双击清空消息窗
					//					MouseArea {
					//						anchors.fill: parent
					//						onDoubleClicked: {
					//							receive_data = ""
					//							//							parent.clear()
					//						}
					//					}
				}
			}
		}
	}
	Rectangle {
		id: back_area
		height: parent.height / 12 <= 40 ? parent.height / 12 : 40
		width: parent.width
		anchors.top: parent.top
		clip: true
		anchors.horizontalCenter: parent.horizontalCenter
		color: "transparent"
		Rectangle {
			height: parent.height / 2
			width: parent.width
			anchors.top: parent.top
			anchors.horizontalCenter: parent.horizontalCenter
			color: "darkgreen"
		}
		Rectangle {
			height: parent.height
			width: parent.width
			anchors.top: parent.top
			clip: true
			anchors.horizontalCenter: parent.horizontalCenter
			color: "darkgreen"
			radius: 200
			///引导返回主窗体事件区域
			MouseArea {
				anchors.fill: parent
				onClicked: click_back()
			}
		}
	}
}

/*##^##
Designer {
	D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
