import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"


/**************************************************
*   文件名：BKeyboard.qml
*   依赖文件: AllBtnStyle.qml
*   本qml文件用途: 触摸屏文本输入
*   作用: 适用于文本符号内容等输入用途
*   设计布局:
*           12*6定位布局为主，首行按键宽高1:1优先，宽度不足采用等宽布局
*           过度高度维持16:9键盘布局
*   使用举例：
*   实例化：
*   BKeyboard {
*   	z: 0//存在默认层高0
*   	keyFontColor:"white"//存在默认字体色
*   	themeColor:"#50cfcf"//存在默认主题色（按键主色）
*   	backgroundColor:"black"//存在默认背景色
*   	backgroundOpacity:0.4//存在默认背景透明度
*   	onExit: {
*   		parent.exit(content, btnState)//返回操作结果以及返回内容
*   	}
*   }
*   文本框以及相关信号使用：
*   ///父级布局器部署信号中转
*   signal inputData(var content)//布局器构建输入信号
*   signal exit(var outputdata, var checkState)//布局器构建返回信号
*   onInputData: {
*       keyTest.inputText(content)//布局器导入数据呼出键盘
*   }
*   ///调用者引用接收
*   onCallKeyboard: {
*   	keyboard_page.exit.connect(receiver)//绑定向键盘所在布局器数据连接点
*       keyboard_page.inputData(inputdata)//向键盘所在布局器导入数据
*   }
*   function receiver(data) {//构建连接点及其接收操作
*       console.debug("re:" + data)//处理接收数据
*       keyboard_page.exit.disconnect(receiver)//断开数据绑定操作，释放连接点
*   }
*   最后修改时间: 2021 / 3 / 12
**************************************************/
Item {
	property string file_version: "1.0.0.0"//文件版本控制
	width: 800
	height: 480
	id: root
	property alias backgroundColor: backboard.color /* 键盘背景颜色 */
	property alias backgroundOpacity: backboard.opacity /* 键盘背景透明度 */
	property color keyFontColor: "white" /* 键盘按键字体颜色 */
	property color themeColor: "#50cfcf" /* 键盘主题颜色 */

	//初始化键盘输入文本
	/* 传入文本信号定义 */
	signal inputText(var content)
	/*普通提醒*/
	signal tip(var outputText)
	/*强提醒*/
	signal reminderTip(var outputText)
	/*btnState:键盘点击按钮 content:键盘输出文本*/
	signal exit(var content, var btnState)

	/* 确认或取消按键信号定义, btnState为true 允许传出文本 */
	anchors.fill: parent
	onInputText: {
		keyboardText.clear()
		keyboardText.insert(keyboardText.cursorPosition, content)
		keyboardText.originalText = content
		keyboardText.focus = true
	}
	MouseArea {
		anchors.fill: parent
		hoverEnabled: true
		onWheel: {

		}
	}
	Rectangle {
		id: backboard
		anchors.fill: parent
		color: "black"
		opacity: 0.4
	}
	Rectangle {
		anchors.centerIn: parent
		width: parent.width
		height: parent.height > parent.width / 16
				* 9 ? parent.width / 16 * 9 : parent.height ///16:9布局控制
		color: "transparent"
		clip: true
		z: 1
		Rectangle {
			anchors.fill: parent
			clip: true
			color: "transparent"
			anchors.leftMargin: parent.width * 0.01
			anchors.rightMargin: anchors.leftMargin
			anchors.topMargin: parent.height * 0.01
			anchors.bottomMargin: anchors.topMargin
			Column {
				id: column
				property int rowsCount: 6
				anchors.fill: parent
				spacing: height * 0.01
				Row {
					///输入行
					width: parent.width
					height: (parent.height - parent.spacing
							 * (parent.rowsCount - 1)) / parent.rowsCount
					property int columnsCount: 3
					spacing: width * 0.01
					property double unitWidth: columnsCount * height <= width ? height : (width - (columnsCount - 1) * spacing) / (columnsCount)
					AllBtnStyle {
						btnImagePath: "../assets/KB/cancel.svg"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							root.exit(keyboardText.originalText, false)
						}
					}
					TextField {
						id: keyboardText
						height: parent.height
						width: parent.width - 2 * (parent.unitWidth + parent.spacing)
						placeholderText: qsTr("Please enter the command")
						font.pointSize: height / 3 <= 0 ? 1 : height / 3
						font.bold: true
						text: ""
						property string originalText: ""
						focus: true
					}
					AllBtnStyle {
						btnImagePath: "../assets/KB/affirm .svg"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							root.exit(keyboardText.text, true)
						}
					}
				}
				Row {
					///Number Row
					width: parent.width
					height: (parent.height - parent.spacing
							 * (parent.rowsCount - 1)) / (parent.rowsCount)
					property int columnsCount: 12
					spacing: width * 0.01
					property double unitWidth: (width - (columnsCount - 1)
												* spacing) / (columnsCount)
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "1" : "!"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "2" : "@"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "3" : "#"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "4" : "$"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "5" : "%"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "6" : "^"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "7" : "&"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "8" : "*"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "9" : "("
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "0" : ")"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "\\" : "|"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						///Delete Key
						btnImagePath: "../assets/KB/del.svg"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						repeaterTimer: true
						onKeySignal: {
							keyboardText.remove(
										keyboardText.cursorPosition - 1,
										keyboardText.cursorPosition)
						}
					}
				}
				Row {
					width: parent.width
					height: (parent.height - parent.spacing
							 * (parent.rowsCount - 1)) / parent.rowsCount
					property int columnsCount: 12
					spacing: width * 0.01
					property double unitWidth: (width - (columnsCount - 1)
												* spacing) / (columnsCount)
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "q" : "Q"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "w" : "W"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "e" : "E"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "r" : "R"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "t" : "T"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "y" : "Y"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "u" : "U"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "i" : "I"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "o" : "O"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "p" : "P"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "[" : "{"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "]" : "}"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
				}
				Row {
					///symbol Row
					width: parent.width
					height: (parent.height - parent.spacing
							 * (parent.rowsCount - 1)) / parent.rowsCount
					property int columnsCount: 12
					spacing: width * 0.01
					property double unitWidth: (width - (columnsCount - 1)
												* spacing) / (columnsCount)
					AllBtnStyle {
						///symbol Key
						id: symbol_Key
						btnTextContent: symbol ? "●" : "☾"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						property bool symbol: false
						onKeySignal: {
							symbol = !symbol
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "a" : "A"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "s" : "S"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "d" : "D"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "f" : "F"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "g" : "G"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "h" : "H"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "j" : "J"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "k" : "K"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "l" : "L"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? ";" : ":"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "\'" : "\""
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
				}
				Row {
					///cap Row
					width: parent.width
					height: (parent.height - parent.spacing
							 * (parent.rowsCount - 1)) / parent.rowsCount
					property int columnsCount: 11
					spacing: width * 0.01
					property double unitWidth: (width - (columnsCount)
												* spacing) / (columnsCount + 1)
					AllBtnStyle {
						id: cap_Key
						btnTextContent: cap ? "CAP" : "cap"
						height: parent.height
						width: parent.unitWidth * 2 + parent.spacing
						btnColor: themeColor
						property bool cap: false
						onKeySignal: {
							cap = !cap
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "z" : "Z"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "x" : "X"
						btnColor: themeColor
						height: parent.height
						width: parent.unitWidth
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "c" : "C"
						btnColor: themeColor
						height: parent.height
						width: parent.unitWidth
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "v" : "V"
						btnColor: themeColor
						height: parent.height
						width: parent.unitWidth
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "b" : "B"
						btnColor: themeColor
						height: parent.height
						width: parent.unitWidth
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "n" : "N"
						btnColor: themeColor
						height: parent.height
						width: parent.unitWidth
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !cap_Key.cap ? "m" : "M"
						btnColor: themeColor
						height: parent.height
						width: parent.unitWidth
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "," : "<"
						btnColor: themeColor
						height: parent.height
						width: parent.unitWidth
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "." : ">"
						btnColor: themeColor
						height: parent.height
						width: parent.unitWidth
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: !symbol_Key.symbol ? "/" : "?"
						btnColor: themeColor
						height: parent.height
						width: parent.unitWidth
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
				}
				Row {
					///space row
					width: parent.width
					height: (parent.height - parent.spacing
							 * (parent.rowsCount - 1)) / parent.rowsCount
					property int columnsCount: 6
					spacing: width * 0.01
					property double unitWidth: (width - (columnsCount + 5)
												* spacing) / (columnsCount + 6)
					AllBtnStyle {
						btnImagePath: "../assets/KB/left.svg"
						height: parent.height
						width: parent.unitWidth * 2 + parent.spacing
						repeaterTimer: true
						btnColor: themeColor
						onKeySignal: {
							keyboardText.cursorPosition -= 1
						}
					}
					AllBtnStyle {
						btnTextContent: symbol_Key.symbol === false ? "、" : "~"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnImagePath: "../assets/KB/blank.svg"
						height: parent.height
						width: parent.unitWidth * 5 + 4 * parent.spacing
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												" ")
						}
					}
					AllBtnStyle {
						btnTextContent: symbol_Key.symbol === false ? "-" : "_"
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnTextContent: symbol_Key.symbol === false ? "+" : "="
						height: parent.height
						width: parent.unitWidth
						btnColor: themeColor
						onKeySignal: {
							keyboardText.insert(keyboardText.cursorPosition,
												str)
						}
					}
					AllBtnStyle {
						btnImagePath: "../assets/KB/right.svg"
						height: parent.height
						width: parent.unitWidth * 2 + parent.spacing
						repeaterTimer: true
						btnColor: themeColor
						onKeySignal: {
							keyboardText.cursorPosition += 1
						}
					}
				}
			}
		}
	}
}
