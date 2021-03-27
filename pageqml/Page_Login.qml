import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

Item {
	id: parentItem
	width: 480
	height: 288
	anchors.fill: parent
	property alias fuzzy: glass.fuzzy
	signal exit
	signal callKeyboard(var inputdata)
	signal setText(var inputdata, var result)
	/*普通提醒*/
	signal tip(var outputText)
	/*强提醒*/
	signal reminderTip(var outputText)
	MouseArea {
		anchors.fill: parent
		hoverEnabled: true
		onWheel: {

		}
		onClicked: {
			exit()
		}
	}
	Glass {
		id: glass
		fuzzy: false
		glasstraget: login_area
	}
	Rectangle {
		id: login_area
		color: glass.fuzzy ? "gray" : "transparent"
		anchors.fill: parent
		Behavior on color {
			ColorAnimation {
				duration: 500
			}
		}
		Rectangle {
			anchors.centerIn: parent
			width: parent.width
			height: parent.height > parent.width / 16
					* 9 ? parent.width / 16 * 9 : parent.height ///16:9布局控制
			color: "transparent"
			Rectangle {
				anchors.fill: parent
				anchors.margins: parent.width * 0.05
				anchors.bottomMargin: parent.height * 0.1
				anchors.topMargin: parent.height * 0.1
				color: "transparent"
				MouseArea {
					anchors.fill: parent
				}
				///User id
				Rectangle {
					id: uid_area
					color: "transparent"
					height: parent.height / 3
					width: parent.width / 4 * 3
					anchors.left: parent.left
					anchors.top: parent.top
					Rectangle {
						anchors.fill: parent
						anchors.margins: 2
						color: "#0dffc7"
						radius: 50
						clip: true
						Label {
							id: uid_title
							anchors.leftMargin: parent.radius / 3
							anchors.left: parent.left
							height: parent.height
							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter
							text: qsTr("User ID : ")
							width: text.length * font.pointSize * 0.4
							font.pointSize: height / 3
						}
						Rectangle {
							anchors.left: uid_title.right
							anchors.rightMargin: parent.radius / 3
							anchors.right: parent.right
							height: parent.height
							color: "transparent"
							TextEdit {
								id: uid_input
								anchors.left: parent.left
								anchors.right: parent.right
								width: parent.width
								horizontalAlignment: Text.AlignHCenter
								verticalAlignment: Text.AlignVCenter
								height: parent.height
								text: "admin"
								color: "#808080"
								//							font.pointSize: height / 3
								font.pointSize: getsize()
								function getsize() {
									//自动计算字体大小
									console.debug(width)
									var pointsize = width / text.length
									if (pointsize >= height)
										pointsize = height * 0.9
									pointsize = pointsize <= 0 ? 1 : pointsize
									return pointsize
								}
								z: 1
								MouseArea {
									anchors.fill: parent
									onClicked: {
										setText.connect(setting)
										callKeyboard(parent.text)
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
				}
				///User pwd
				Rectangle {
					id: pwd_area
					color: "transparent"
					height: parent.height / 3
					width: parent.width / 4 * 3
					anchors.left: parent.left
					anchors.top: uid_area.bottom
					Rectangle {
						anchors.fill: parent
						anchors.margins: 2
						color: "#0dffc7"
						radius: 50
						clip: true
						Label {
							id: pwd_title
							anchors.leftMargin: parent.radius / 3
							anchors.left: parent.left
							height: parent.height
							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter
							text: qsTr("Password : ")
							width: text.length * font.pointSize * 0.4
							font.pointSize: height / 3
						}
						Rectangle {
							anchors.left: pwd_title.right
							anchors.rightMargin: parent.radius / 3
							anchors.right: parent.right
							height: parent.height
							color: "transparent"
							//							border.width: 1
							TextEdit {
								id: pwd_input
								anchors.left: parent.left
								horizontalAlignment: Text.AlignHCenter
								verticalAlignment: Text.AlignVCenter
								width: parent.width
								height: parent.height
								text: "123456"
								color: "#808080"
								//							font.pointSize: height / 3
								font.pointSize: getsize()
								function getsize() {
									//自动计算字体大小
									var pointsize = width / text.length * 1.1
									if (pointsize >= height)
										pointsize = height * 0.9
									pointsize = pointsize <= 0 ? 1 : pointsize
									return pointsize
								}
								z: 1
								MouseArea {
									anchors.fill: parent
									onClicked: {
										setText.connect(setting)
										callKeyboard(parent.text)
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
				}
				///IP Port
				Rectangle {
					id: network_area
					color: "transparent"
					height: parent.height / 3
					width: parent.width / 4 * 3
					anchors.left: parent.left
					anchors.top: pwd_area.bottom
					Rectangle {
						anchors.fill: parent
						anchors.margins: 2
						color: "#0dffc7"
						radius: 50
						clip: true
						Label {
							anchors.leftMargin: parent.radius / 3
							id: network_title
							anchors.left: parent.left
							height: parent.height
							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter
							text: qsTr("[IP:Port] : ")
							width: text.length * font.pointSize * 0.4
							font.pointSize: height / 3
						}
						Rectangle {
							anchors.left: network_title.right
							anchors.rightMargin: parent.radius / 3
							anchors.right: parent.right
							height: parent.height
							color: "transparent"
							//IP
							TextEdit {
								id: ip_input
								anchors.left: parent.left
								//								anchors.right: character.left
								horizontalAlignment: Text.AlignHCenter
								verticalAlignment: Text.AlignVCenter
								height: parent.height
								text: "127.0.0.1"
								color: "#808080"
								//							font.pointSize: height / 3.5
								font.pointSize: getsize()
								function getsize() {
									//自动计算字体大小
									var pointsize = width / text.length * 1.1
									if (pointsize >= height)
										pointsize = height * 0.9
									pointsize = pointsize <= 0 ? 1 : pointsize
									return pointsize
								}
								width: parent.width * 0.7
								z: 1
								MouseArea {
									anchors.fill: parent
									onClicked: {
										setText.connect(setting)
										callKeyboard(parent.text)
									}
									function setting(data, result) {
										if (result) {
											parent.text = data
										}
										setText.disconnect(setting)
									}
								}
							}
							Label {
								id: character
								anchors.right: port_input.left
								height: parent.height
								horizontalAlignment: Text.AlignHCenter
								verticalAlignment: Text.AlignVCenter
								text: " : "
								width: parent.width * 0.1
								opacity: port_input.text.length < 1 ? 0 : 1
								font.pointSize: height / 3
							}
							//Port
							TextEdit {
								id: port_input
								anchors.right: parent.right
								horizontalAlignment: Text.AlignHCenter
								verticalAlignment: Text.AlignVCenter
								height: parent.height
								color: "#808080"
								text: "80"
								width: parent.width * 0.2
								z: 1
								font.pointSize: getsize()
								function getsize() {
									//自动计算字体大小
									var pointsize = width / text.length * 1.1
									if (pointsize >= height)
										pointsize = height * 0.9
									pointsize = pointsize <= 0 ? 1 : pointsize
									return pointsize
								}
								MouseArea {
									anchors.fill: parent
									onClicked: {
										setText.connect(setting)
										callKeyboard(parent.text)
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
				}
				///Btn Login
				Rectangle {
					id: loginBtn_area
					color: "transparent"
					height: parent.height / 3
					width: parent.width / 4
					anchors.right: parent.right
					anchors.top: parent.top
					AllBtnStyle {
						btnTextContent: qsTr("Login")
						btnImagePath: ""
						anchors.fill: parent
						anchors.margins: 2
						btnColor: "#8efc60"
						radius: 10
						onKeySignal: {
							parentItem.tip(
										ip_input.text
										+ (port_input.text.trim().length
										   > 0 ? (port_input.text.trim(
													  ) === "80" ? "" : (":" + port_input.text.trim(
																			 ))) : "")) //打印地址
						}
					}
				}
				///Btn Cancel
				Rectangle {
					id: cancelBtn_area
					color: "transparent"
					height: parent.height / 3
					width: parent.width / 4
					anchors.right: parent.right
					anchors.top: loginBtn_area.bottom
					AllBtnStyle {
						btnTextContent: qsTr("Cancel")
						btnImagePath: ""
						anchors.fill: parent
						anchors.margins: 2
						btnColor: "#fc6060"
						radius: 10
						onKeySignal: {

						}
					}
				}
				///Autologin
				Rectangle {
					id: autologin_area
					color: "#96fed3"
					radius: 20
					clip: true
					height: parent.height / 6
					width: parent.width / 4
					anchors.right: parent.right
					anchors.bottom: remember_area.top
					CheckBox {
						id: autologin_check
						anchors.fill: parent
						anchors.margins: 3
						text: qsTr("AutoLogin")
						font.pointSize: height / 3.5
						checked: true
					}
				}
				///Remember me
				Rectangle {
					id: remember_area
					color: "#96fed3"
					radius: 20
					clip: true
					height: parent.height / 6
					width: parent.width / 4
					anchors.right: parent.right
					anchors.bottom: parent.bottom
					CheckBox {
						id: remember_check
						anchors.fill: parent
						anchors.margins: 3
						text: qsTr("Remember")
						font.pointSize: height / 3.5
						enabled: !autologin_check.checked
						checked: autologin_check.checked === true ? true : true
					}
				}
			}
		}
	}
}
