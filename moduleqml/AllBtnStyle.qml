import QtQuick 2.9
import QtQml 2.2
import "../pageqml"
import "../moduleqml"
import "../"


/**************************************************
*   文件名：AllBtnStyle.qml
*   本qml文件用途: 通用按键的样式
*   可以使用的别名接口
*   适用于纯文字 / 纯图片 / 文字与图片并存的按键
*   使用举例:
*   AllBtnStyle
*       {
*           根据下面两项决定按键样式
*           btnImagePath: "qrc:/cancel.svg"     按键图片路径 默认为 ""
*           btnTextContent: "起飞3"				按键文本内容 默认为 ""
*           btnColor: "lime"                    按键主题颜色 默认为 lightgreen
*           btnTextFontColor: "white"           字体颜色 默认为白色
*           btnAnimation: 0                     按键动画类型 默认为缩放扩张 = 0 旋转 = 1 默认为0
*           repeaterTimer                       按键重复是否启动(间隔100ms) 默认false
*           rotateAnimRepeater: false           旋转动画运行状态 true: 连续旋转 false: 取消旋转 默认为false
*       }
*   可以使用的信号:
*   keySignal(var str)  按键点击信号
*   最后修改时间: 2021 / 3 / 18
*	》》文本大小自动
**************************************************/
Item {
	id: myitem
	width: 200
	height: 200
	property color btnColor: "lightgreen" /* 按键颜色 */
	property url btnImagePath: "" /* 按键图片路径 */
	property int btnAnimation: 0 /* 按键动画类型 默认为缩放扩张 = 0 旋转 = 1 默认为0 */
	property alias btnTextFontColor: btnText.color /* 文本字体 颜色 内容 */
	property alias btnTextContent: btnText.text /* 按键文本 */
	property alias radius: realBtn.radius /* 按键圆角 */
	property int defaultZ: 1 /* 默认层级 */
	property bool repeaterTimer: false /* 按键重复是否启动 */
	property alias repeaterInterval: repeatClick.interval /* 按键输出重复间隔:默认100ms */
	property bool rotateAnimRepeater: false /* 旋转动画运行状态 true: 连续旋转 false: 取消旋转 */

	signal keySignal(var str)

	/* 按键点击信号 */
	//	anchors.fill: parent
	z: defaultZ
	///Resolution test
	//	Text {
	//		text: parent.width + "*" + parent.height
	//		anchors.left: parent.left
	//		anchors.top: parent.top
	//		z: 10
	//	}
	//键盘长按定时器
	Timer {
		id: repeatClick
		repeat: true
		interval: 100
		triggeredOnStart: true
		onTriggered: {
			myitem.keySignal(myitem.btnTextContent)
		}
	}

	Rectangle //按键容器
	{
		color: "transparent"
		anchors.fill: parent
		Rectangle {
			//实体按键
			id: realBtn
			property double defaultProportion: 1
			property double proportion: defaultProportion

			//默认值
			color: btnColor
			width: parent.width * proportion
			height: parent.height * proportion
			anchors.centerIn: parent
			radius: 8
			clip: true

			Rectangle {
				//IMG
				color: "transparent"
				anchors.top: parent.top
				width: parent.width
				height: btnTextContent === "" ? parent.height : parent.height * 0.7
				clip: true
				Rectangle {
					color: "transparent"
					anchors.fill: parent
					anchors.leftMargin: 0
					anchors.rightMargin: 0
					anchors.topMargin: parent.height * 0.1
					anchors.bottomMargin: anchors.topMargin
					clip: true
					Image {
						id: btnImage
						anchors.fill: parent
						source: btnImagePath
						fillMode: Image.PreserveAspectFit
						property int animationTime: 500
						Behavior on rotation {
							id: rotationAnimation
							NumberAnimation {
								duration: btnImage.animationTime
							}
						}

						Timer {
							onRunningChanged: {
								if (running) {
									///调零
									if (btnImage.rotation > 360) {
										rotationAnimation.enabled = false
										while (btnImage.rotation > 360) {
											btnImage.rotation -= 360
										}
										rotationAnimation.enabled = true
									}
									btnImage.rotation += btnImage.rotation % 360
											=== 0 ? 360 : 360 - btnImage.rotation % 360
								}
							}
							running: rotateAnimRepeater
							repeat: true
							interval: btnImage.animationTime
							onTriggered: {
								if (btnAnimation === 1 && rotateAnimRepeater) {
									///调零
									if (btnImage.rotation > 360) {
										rotationAnimation.enabled = false
										while (btnImage.rotation > 360) {
											btnImage.rotation -= 360
										}
										rotationAnimation.enabled = true
									}
									btnImage.rotation += (360 - btnImage.rotation % 360) > 0
											&& (360 - btnImage.rotation % 360) < 60 ? (360 - btnImage.rotation % 360 + 360) : (360 - btnImage.rotation % 360)
								}
							}
						}

						Connections {
							target: myitem
							onKeySignal: {
								//当动画类型为旋转、不允许重复并且图片角度正好是360时, 旋转图片一次
								if (btnAnimation === 1
										&& rotateAnimRepeater === false
										&& btnImage.rotation % 360 === 0) {
									///调零
									rotationAnimation.enabled = false
									btnImage.rotation = 0
									rotationAnimation.enabled = true

									btnImage.rotation += 360
								}
							}
						}
					}
				}
			}
			Rectangle {
				//Text
				color: "transparent"
				anchors.bottom: parent.bottom
				width: parent.width
				height: btnImagePath.toString().trim(
							) === "" ? parent.height : parent.height * 0.3
				clip: true
				Rectangle {
					color: "transparent"
					anchors.fill: parent
					anchors.leftMargin: 0
					anchors.rightMargin: anchors.leftMargin
					anchors.topMargin: parent.height * 0.1
					anchors.bottomMargin: anchors.topMargin
					clip: true
					Text {
						id: btnText
						color: "white"
						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter
						clip: true
						font.pointSize: getsize()
						font.bold: true
						anchors.fill: parent
						function getsize() {
							//自动计算字体大小
							var pointsize = width / text.length * 1.1
							if (pointsize >= height)
								pointsize = height * 0.9
							pointsize = pointsize <= 0 ? 1 : pointsize
							return pointsize
						}
					}
				}
			}
		}
	}
	MouseArea {
		id: btnMouseArea
		anchors.fill: parent
		hoverEnabled: true
		onEntered: {

		}
		onExited: {

		}
		onClicked: {

			//触发按键信号
			//			myitem.keySignal(myitem.btnTextContent)
		}

		//长按启动按键重复定时器
		onPressAndHold: {
			if (repeaterTimer === true) {
				repeatClick.start()
			}
		}

		//长按结束时关闭按键重复定时器
		onReleased: {
			if (repeaterTimer === true) {
				if (repeatClick.running)
					repeatClick.stop()
				else
					myitem.keySignal(myitem.btnTextContent)
			} else {
				if (myitem.state != "exitState")
					myitem.keySignal(myitem.btnTextContent)
			}
		}
	}

	state: "exitState"
	//containsMouse(鼠标光标在区域内生效), containsPress(鼠标光标在区域内且按下生效)
	states: [
		State {
			name: "enterState"
			when: btnMouseArea.containsMouse && (!btnMouseArea.containsPress
												 || btnAnimation === 1)
			PropertyChanges {
				target: realBtn
				proportion: 0.8
			}
			PropertyChanges {
				target: myitem
				z: defaultZ + 1
			}
		},
		State {
			name: "exitState"
			when: !btnMouseArea.containsMouse
			PropertyChanges {
				target: realBtn
				proportion: defaultProportion
			}
			PropertyChanges {
				target: myitem
				z: defaultZ
			}
		},
		State {
			name: "clickState"
			when: btnMouseArea.containsPress && btnAnimation === 0
			PropertyChanges {
				target: realBtn
				proportion: 0.5
			}
			PropertyChanges {
				target: myitem
				z: defaultZ + 1
			}
		}
	]
	transitions: [
		Transition {
			from: "exitState"
			to: "enterState"
			NumberAnimation {
				target: realBtn
				properties: "proportion"
				duration: 100
			}
		},
		Transition {
			to: "exitState"
			NumberAnimation {
				target: realBtn
				properties: "proportion"
				duration: 100
			}
		},
		Transition {
			//clickState Release
			from: "clickState"
			to: "enterState"

			SequentialAnimation {
				NumberAnimation {
					target: realBtn
					properties: "proportion"
					to: 1.2
					duration: 100
				}
				NumberAnimation {
					target: realBtn
					properties: "proportion"
					to: 0.8
					duration: 100
				}
			}
		},
		Transition {
			//clickState Press
			from: "enterState"
			to: "clickState"
			NumberAnimation {
				target: realBtn
				properties: "proportion"
				duration: 100
			}
		}
	]
}
