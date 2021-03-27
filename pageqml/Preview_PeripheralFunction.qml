import QtQuick 2.9
import "../pageqml"
import "../moduleqml"
import "../"

//import QtQuick.Controls 2.2
Item {
	id:parentItem
	property double itemsProportion: 0.25
	property int limit_size: 100
	/*普通提醒*/
	signal tip(var outputText)
	/*强提醒*/
	signal reminderTip(var outputText)
	height: 280
	width: 480
	Preview_item {
		id: hotend_area
		itemDirection: 0
		itemIndex: 0
		themeColor: "#ba1717"
		itemProportion: parent.itemsProportion
		limit_size: parent.limit_size
		image: "qrc:/assets/heat-gas.svg"
		text: "999℃"
		onReminderTip: parentItem.reminderTip(outputText)
		onTip: parentItem.tip(outputText)
	}
	Preview_item {
		id: bed_area
		itemDirection: 0
		itemIndex: 1
		themeColor: "#987212"
		itemProportion: parent.itemsProportion
		limit_size: parent.limit_size
		image: "qrc:/assets/heat-bed.svg"
		text: "999℃"
		onReminderTip: parentItem.reminderTip(outputText)
		onTip: parentItem.tip(outputText)
	}
	Preview_item {
		id: fan_area
		itemDirection: 0
		itemIndex: 2
		themeColor: "#90d805"
		itemProportion: parent.itemsProportion
		limit_size: parent.limit_size
		image: "qrc:/assets/kongzhiqi_fengji.svg"
		text: "125%"
		onReminderTip: parentItem.reminderTip(outputText)
		onTip: parentItem.tip(outputText)
	}
	Preview_item {
		id: state_area
		itemDirection: 0
		itemIndex: 3
		themeColor: "#6a2e19"
		itemProportion: parent.itemsProportion
		limit_size: parent.limit_size
		image: "qrc:/assets/print.svg"
		text: "125%"
		onReminderTip: parentItem.reminderTip(outputText)
		onTip: parentItem.tip(outputText)
	}
	Preview_item {
		id: setting_area
		itemDirection: 1
		itemIndex: 0
		themeColor: "#b82e74"
		itemProportion: itemsProportion
		limit_size: parent.limit_size
		image: "qrc:/assets/settings.svg"
		onReminderTip: parentItem.reminderTip(outputText)
		onTip: parentItem.tip(outputText)
	}
	Preview_item {
		id: network_area
		itemDirection: 1
		itemIndex: 1
		themeColor: "#1a7e7f"
		itemProportion: parent.itemsProportion
		limit_size: parent.limit_size
		image: "qrc:/assets/yuanchengkongzhi.svg"
		onReminderTip: parentItem.reminderTip(outputText)
		onTip: parentItem.tip(outputText)
	}
	Preview_item {
		id: security_area
		itemDirection: 1
		itemIndex: 2
		themeColor: "#10585f"
		itemProportion: parent.itemsProportion
		limit_size: parent.limit_size
		image: "qrc:/assets/guanji.svg"
		onReminderTip: parentItem.reminderTip(outputText)
		onTip: parentItem.tip(outputText)
	}
	Preview_item {
		id: other_area
		itemDirection: 1
		itemIndex: 3
		themeColor: "#550f6c"
		itemProportion: parent.itemsProportion
		limit_size: parent.limit_size
		image: "qrc:/assets/camera.svg"
		onReminderTip: parentItem.reminderTip(outputText)
		onTip: parentItem.tip(outputText)
	}
}
