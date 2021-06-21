import QtQuick 2.9
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

//import QtQuick.Controls 2.2
Item {
    property string file_version: "1.0.0.0" //文件版本控制
    id: root
    property double itemsProportion: 0.25
    property int limit_size: 100
    /*普通提醒*/
    signal tip(var outputText)
    /*强提醒*/
    signal reminderTip(var outputText)
    height: 280
    width: 480

    property var currentInfo
    Connections {
        target: myWebsoket
        // @disable-check M16
        onWsCurrentInfo: {
            root.getcurrentInfo(json)
        }
    }
    function getcurrentInfo(jsonpacket) {
        root.currentInfo = JSON.parse(jsonpacket)
    }

    Preview_item {
        id: hotend_area
        itemDirection: 0
        itemIndex: 0
        themeColor: "#ba1717"
        itemProportion: parent.itemsProportion
        limit_size: parent.limit_size
        image: "../assets/heat-gas.svg"
        text: root.currentInfo.current.state.flags.operational
              === true ? root.currentInfo.current.temps[0].tool0.actual + "℃" : ""
        onReminderTip: root.reminderTip(outputText)
        onTip: root.tip(outputText)
    }
    Preview_item {
        id: bed_area
        itemDirection: 0
        itemIndex: 1
        themeColor: "#987212"
        itemProportion: parent.itemsProportion
        limit_size: parent.limit_size
        image: "../assets/heat-bed.svg"
        text: root.currentInfo.current.state.flags.operational
              === true ? root.currentInfo.current.temps[0].bed.actual + "℃" : ""
        onReminderTip: root.reminderTip(outputText)
        onTip: root.tip(outputText)
    }
    Preview_item {
        id: fan_area
        itemDirection: 0
        itemIndex: 2
        themeColor: "#90d805"
        itemProportion: parent.itemsProportion
        limit_size: parent.limit_size
        image: "../assets/kongzhiqi_fengji.svg"
        //        text: root.currentInfo.current.temps.tool0.actual
        onReminderTip: root.reminderTip(outputText)
        onTip: root.tip(outputText)
    }
    Preview_item {
        id: state_area
        itemDirection: 0
        itemIndex: 3
        themeColor: "#6a2e19"
        itemProportion: parent.itemsProportion
        limit_size: parent.limit_size
        image: "../assets/print.svg"
        text: root.currentInfo.current.state.flags.operational === true
              && root.currentInfo.current.progress.completion
              !== null ? root.currentInfo.current.progress.completion + "%" : ""
        onReminderTip: root.reminderTip(outputText)
        onTip: root.tip(outputText)
    }
    Preview_item {
        id: setting_area
        itemDirection: 1
        itemIndex: 0
        themeColor: "#b82e74"
        itemProportion: itemsProportion
        limit_size: parent.limit_size
        image: "../assets/settings.svg"
        onReminderTip: root.reminderTip(outputText)
        onTip: root.tip(outputText)
    }
    Preview_item {
        id: network_area
        itemDirection: 1
        itemIndex: 1
        themeColor: "#1a7e7f"
        itemProportion: parent.itemsProportion
        limit_size: parent.limit_size
        image: "../assets/yuanchengkongzhi.svg"
        onReminderTip: root.reminderTip(outputText)
        onTip: root.tip(outputText)
    }
    Preview_item {
        id: security_area
        itemDirection: 1
        itemIndex: 2
        themeColor: "#10585f"
        itemProportion: parent.itemsProportion
        limit_size: parent.limit_size
        image: "../assets/guanji.svg"
        onReminderTip: root.reminderTip(outputText)
        onTip: root.tip(outputText)
    }
    Preview_item {
        id: other_area
        itemDirection: 1
        itemIndex: 3
        themeColor: "#550f6c"
        itemProportion: parent.itemsProportion
        limit_size: parent.limit_size
        image: "../assets/camera.svg"
        onReminderTip: root.reminderTip(outputText)
        onTip: root.tip(outputText)
    }
}
