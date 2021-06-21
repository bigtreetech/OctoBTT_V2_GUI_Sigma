import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

Item {
	property string file_version: "1.0.0.0"//文件版本控制
	id: root
	/*普通提醒*/
	signal tip(var outputText)
	/*强提醒*/
	signal reminderTip(var outputText)
	property color borderColor
	property double borderWidth
	Rectangle {
		id: rect
		anchors.fill: parent
		color: "transparent"
		border.color: borderColor
		border.width: borderWidth
		Item {
			anchors.centerIn: parent
			id: baseItem
			height: (parent.width > parent.height ? parent.height : parent.width) - 2 * borderWidth
			width: height
			signal colorChanged(string newColor)
			property int circleWidth: width / 10 //圆环宽度
			property var curColor: undefined

			Rectangle {
				id: control
				width: baseItem.width
				height: width
				color: "transparent"
				//				border.width: 1
				//				border.color: "black"
				anchors.margins: 10

				// 根据角度获取颜色值
				function getAngleColor(angle) {
					var color, d
					if (angle < Math.PI * 2 / 5) {
						// angle: 0-72
						d = 255 / (Math.PI * 2 / 5) * angle
						color = '255,' + Math.round(
									d) + ',0' // color: 255,0,0 - 255,255,0
					} else if (angle < Math.PI * 4 / 5) {
						// angle: 72-144
						d = 255 / (Math.PI * 2 / 5) * (angle - Math.PI * 2 / 5)
						color = (255 - Math.round(
									 d)) + ',255,0' // color: 255,255,0 - 0,255,0
					} else if (angle < Math.PI * 6 / 5) {
						// angle: 144-216
						d = 255 / (Math.PI * 2 / 5) * (angle - Math.PI * 4 / 5)
						color = '0,255,' + Math.round(
									d) // color: 0,255,0 - 0,255,255
					} else if (angle < Math.PI * 8 / 5) {
						// angle: 216-288
						d = 255 / (Math.PI * 2 / 5) * (angle - Math.PI * 6 / 5)
						color = '0,' + (255 - Math.round(
											d)) + ',255' // color: 0,255,255 - 0,0,255
					} else {
						// angle: 288-360
						d = 255 / (Math.PI * 2 / 5) * (angle - Math.PI * 8 / 5)
						color = Math.round(
									d) + ',0,' + (255 - Math.round(
													  d)) // color: 0,0,255 - 255,0,0
					}
					return color
				}

				// 获取旋转角度
				function getRotateAngle(mouseX, mouseY) {
					var yPosOffset = mouseY - control.width / 2
					// 计算角度 : tan(x) = (y2-y1)/(x2-x1);
					var xPosOffset = mouseX - control.height / 2
					// 旋转的弧度 hudu, 角度angle
					var hudu = 0, angle = 0
					if (xPosOffset !== 0 && yPosOffset !== 0) {
						hudu = Math.atan(Math.abs(yPosOffset / xPosOffset))
					}

					if (xPosOffset === 0 && yPosOffset === 0) {
						return angle
					} else if (xPosOffset < 0 && yPosOffset < 0) {
						angle = hudu * 180 / Math.PI // 左上
					} else if (xPosOffset === 0 && yPosOffset < 0) {
						angle = 90 // 上 中间
					} else if (xPosOffset > 0 && yPosOffset < 0) {
						angle = 180 - hudu * 180 / Math.PI // 右上
					} else if (xPosOffset > 0 && yPosOffset === 0) {
						angle = 180 // 上 下 中间
					} else if (xPosOffset > 0 && yPosOffset > 0) {
						angle = 180 + hudu * 180 / Math.PI // 右下
					} else if (xPosOffset === 0 && yPosOffset > 0) {
						angle = 270 // 下 中间
					} else if (xPosOffset < 0 && yPosOffset > 0) {
						angle = 360 - hudu * 180 / Math.PI // 左下
					}
					return angle
				}

				// 通过鼠标所在点更新Canvas画图信息
				function updateCanvasByMousePos(x, y) {
					var currentAngle = control.getRotateAngle(x, y)
					console.log(x, y, currentAngle)
					updateCanvasByAngle(currentAngle)
				}

				//通过角度更新Canvas画图信息位置
				function updateCanvasByAngle(angle) {
					var newX = control.width / 2 + -Math.cos(
								angle * Math.PI / 180)
							* (control.width / 2 - baseItem.circleWidth / 2 - 2
							   * control.anchors.margins)
					var newY = control.height / 2 - Math.sin(
								angle * Math.PI / 180)
							* (control.height / 2 - baseItem.circleWidth / 2 - 2
							   * control.anchors.margins)

					console.log("new : ", newX, newY,
								"\ncur color is :" + baseItem.curColor)
					handle.xDrawPos = newX
					handle.yDrawPos = newY
					handle.requestPaint()

					centerPanel.requestPaint()

					baseItem.curColor = 'rgb(' + control.getAngleColor(
								((angle + 180) % 360) / 180 * Math.PI) + ')'
					baseItem.colorChanged(baseItem.curColor) //发信号
				}

				// 鼠标选择圆环按钮
				Canvas {
					id: handle
					width: parent.width
					height: width

					property int xDrawPos: 0
					property int yDrawPos: 0

					onPaint: {
						var ctx = getContext("2d")
						ctx.clearRect(0, 0, width, height)
						ctx.beginPath()
						ctx.arc(xDrawPos, yDrawPos, baseItem.circleWidth + 1,
								0, 2 * Math.PI, false)
						ctx.fillStyle = 'darkgray'
						//						ctx.fillStyle = baseItem.curColor
						ctx.fill()
						ctx.strokeStyle = 'transparent'
						ctx.stroke()
						ctx.closePath()

						ctx.beginPath()
						ctx.arc(xDrawPos, yDrawPos,
								baseItem.circleWidth / 1 - 2, 0,
								2 * Math.PI, false)
						ctx.fillStyle = baseItem.curColor
						ctx.fill()
						ctx.strokeStyle = 'transparent'
						ctx.stroke()
						ctx.closePath()
					}

					z: 1000
				}
				Canvas {
					id: centerPanel
					width: parent.width - 4 * control.anchors.margins
					height: width
					anchors.centerIn: parent
					z: canvas.z + 100
					onPaint: {
						var ctx = getContext("2d")

						ctx.clearRect(0, 0, width, height)
						ctx.beginPath()
						//						ctx.moveTo(0, 0)
						ctx.arc(width / 2, height / 2,
								width / 2 - baseItem.circleWidth * 2, 0,
								2 * Math.PI, false)
						ctx.fillStyle = baseItem.curColor
						ctx.fill()
						ctx.strokeStyle = 'transparent'
						ctx.stroke()
						ctx.closePath()
					}
				}
				// 圆环画布
				Canvas {
					id: canvas
					width: parent.width - 4 * control.anchors.margins
					height: width
					anchors.centerIn: parent

					onPaint: {
						var ctx = getContext("2d")
						var iSectors = 360
						var iSectorAngle = (360 / iSectors) / 180 * Math.PI

						// in radians
						ctx.translate(width / 2, height / 2)
						for (var i = 0; i < iSectors; i++) {
							var startAngle = 0
							var endAngle = startAngle + iSectorAngle
							var radius = (width / 2 - 1)
							var color = control.getAngleColor(iSectorAngle * i)
							ctx.beginPath()
							ctx.moveTo(0, 0)
							ctx.arc(0, 0, radius, startAngle, endAngle, false)
							ctx.closePath()
							ctx.strokeStyle = 'rgb(' + color + ')'
							ctx.stroke()
							ctx.fillStyle = 'rgb(' + color + ')'
							ctx.fill()
							ctx.rotate(iSectorAngle)
						}

						ctx.restore()
						ctx.save()
						ctx.translate(0, 0)
						ctx.beginPath()
						ctx.arc(0, 0, width / 2 - baseItem.circleWidth, 0,
								2 * Math.PI, false)
						ctx.fillStyle = 'white'
						ctx.fill()
						ctx.strokeStyle = 'transparent'
						ctx.stroke()
						ctx.restore()
					}

					MouseArea {
						id: colorSelectorMouseArea
						anchors.fill: parent
						onMouseXChanged: {
							control.updateCanvasByMousePos(mouseX, mouseY)
						}
					}

					Component.onCompleted: {
						control.updateCanvasByAngle(0)
					}
				}
			}
		}
		///单元动画组
		Behavior on opacity {
			NumberAnimation {
				duration: 500
			}
		}
	}
}

/*##^##
Designer {
	D{i:0;autoSize:true;formeditorZoom:0.5;height:480;width:640}
}
##^##*/

