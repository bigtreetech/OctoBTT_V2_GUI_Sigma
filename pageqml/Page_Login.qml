import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQml 2.0

import "../pageqml"
import "../moduleqml"
import "../"

import UserLoginModule 1.0

Item {
    property string file_version: "1.0.0.0" //文件版本控制
    id: root
    width: 480
    height: 288
    anchors.fill: parent
    property UserLoginModule userLoginModule: userLoginModule
    property alias fuzzy: glass.fuzzy
    property bool needRegister: true
    signal exit
    signal callKeyboard(var inputdata)
    signal setText(var inputdata, var result)
    /*普通提醒*/
    signal tip(var outputText)
    /*强提醒*/
    signal reminderTip(var outputText)

    Component.onCompleted: {
        userLoginModule.initialize()
    }
    UserLoginModule {
        id: userLoginModule
        onIpCase: {
            if (online) {
                network_panel.border.color = "transparent"
                ip_input.color = "#808080"
                network_panel.border.width = 1
            } else {
                network_panel.border.color = "red"
                ip_input.color = "red"
                network_panel.border.width = 5
            }
        }
        onUserLoginCase: {
            console.debug("loginState:" + loginState)
            switch (loginState) {
            case 0x111:
            case 0x100:
                //0x100:免注册登录直接使用
                //0x111:登陆成功
                login_page.state = "default"
                break
            default:
                login_page.state = "Show"
                break
            }
            switch (loginState) {
            case 0x000:
                //0x000:需要登陆
                if (userLoginModule.autoLogin === true)
                    sendUserLogin()
                else {
                    console.debug("需要登陆")
                    loginBtn_area.state = "login_Mode"
                }
                break
            case 0x001:
                //0x001:登陆失败,需要登陆
                console.debug("登陆失败")
                root.reminderTip(qsTr("Incorrect account or password!"))
                loginBtn_area.state = "login_Mode"
                break
            case 0x002:
                //0x002:退出成功
                console.debug("退出成功")
                loginBtn_area.state = "login_Mode"
                break
            case 0x100:
                //0x100:免注册登录直接使用
                console.debug("免注册登录直接使用")
                if (loginBtn_area.state !== "initialize_Mode")
                    root.tip(qsTr("You don't need to login!"))
                loginBtn_area.state = "admins_Mode"
                break
            case 0x110:
                //0x110:需要注册
                console.debug("需要注册")
                root.tip(qsTr("Please confirm your registration information."))
                loginBtn_area.state = "register_Mode"
                break
            case 0x220:
                //0x220:注册成功
                console.debug("注册成功")
                //                loginBtn_area.state = "login_Mode"
                //                root.tip(qsTr("Registered successfully."))
                sendUserLogin()
                break
            case 0x120:
                //0x220:注册失败
                console.debug("注册失败")
                loginBtn_area.state = "detection_Mode"
                root.tip(qsTr("Registration failed. Please try again."))
                break
            case 0x111:
                //0x111:登陆成功
                console.debug("登陆成功")
                if (loginBtn_area.state !== "initialize_Mode")
                    root.tip(qsTr("Please enjoy!"))
                loginBtn_area.state = "logout_Mode"
                break
            case 0xfff:
                //0xfff:网络地址错误
                console.debug("网络地址错误")
                root.reminderTip(
                            qsTr("Network timeout or invalid network address!"))
                loginBtn_area.state = "detection_Mode"
                break
            }

            switch (loginState) {
            case 0x111:
            case 0x100:
                //0x100:免注册登录直接使用
                //0x111:登陆成功
                functionModule.getSettings()
                break
            default:
                break
            }
        }
    }
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
                    height: parent.height / 4
                    width: parent.width
                    anchors.left: parent.left
                    anchors.top: parent.top
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 2
                        color: uid_area.enabled ? "#0dffc7" : "#A0A0A0"
                        Behavior on color {
                            ColorAnimation {
                                duration: 500
                            }
                        }
                        radius: 50
                        clip: true
                        Label {
                            id: uid_title
                            anchors.leftMargin: parent.height / 2
                            anchors.left: parent.left
                            height: parent.height
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: qsTr("User ID : ")
                            width: contentWidth
                            font.pointSize: getsize()
                            function getsize() {
                                //自动计算字体大小
                                var pointsize = height
                                if (pointsize >= (height * 0.4))
                                    pointsize = height * 0.4
                                pointsize = pointsize <= 0 ? 1 : pointsize
                                return pointsize
                            }
                        }
                        Rectangle {
                            anchors.left: uid_title.right
                            anchors.rightMargin: parent.radius / 3
                            anchors.right: parent.right
                            height: parent.height
                            color: "transparent"
                            TextEdit {
                                id: uid_input
                                enabled: loginBtn_area.state !== "admins_Mode"
                                anchors.left: parent.left
                                anchors.right: parent.right
                                width: parent.width
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                height: parent.height
                                text: globalAccess.userinfo.uid
                                color: text.length < 5 && loginBtn_area.state
                                       === "register_Mode" ? "#FF0000" : "#808080"
                                font.pointSize: getsize()
                                Behavior on color {
                                    ColorAnimation {
                                        duration: 500
                                    }
                                }
                                function getsize() {
                                    //自动计算字体大小
                                    var pointsize = width / text.length
                                    if (pointsize >= (height * 0.7))
                                        pointsize = height * 0.7
                                    pointsize = pointsize <= 0 ? 1 : pointsize
                                    return pointsize
                                }
                                z: 1
                                //                                onTextChanged: font.pointSize = getsize()
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.IBeamCursor
                                    onClicked: {
                                        setText.connect(setting)
                                        callKeyboard(parent.text)
                                    }
                                    function setting(data, result) {
                                        if (result) {
                                            globalAccess.userinfo.uid = data
                                            //                                            parent.text = data
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
                    height: parent.height / 4
                    width: parent.width
                    anchors.left: parent.left
                    anchors.top: uid_area.bottom
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 2
                        color: pwd_area.enabled ? "#0dffc7" : "#A0A0A0"
                        Behavior on color {
                            ColorAnimation {
                                duration: 500
                            }
                        }
                        radius: 50
                        clip: true
                        Label {
                            id: pwd_title
                            anchors.leftMargin: parent.height / 2
                            anchors.left: parent.left
                            height: parent.height
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: qsTr("Password : ")
                            width: contentWidth
                            font.pointSize: getsize()
                            function getsize() {
                                //自动计算字体大小
                                var pointsize = height
                                if (pointsize >= (height * 0.4))
                                    pointsize = height * 0.4
                                pointsize = pointsize <= 0 ? 1 : pointsize
                                return pointsize
                            }
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
                                enabled: loginBtn_area.state !== "admins_Mode"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                width: parent.width
                                height: parent.height
                                text: globalAccess.userinfo.pwd
                                color: text.length < 6 && loginBtn_area.state
                                       === "register_Mode" ? "#FF0000" : "#808080"
                                //							font.pointSize: height / 3
                                font.pointSize: getsize()
                                Behavior on color {
                                    ColorAnimation {
                                        duration: 500
                                    }
                                }
                                function getsize() {
                                    //自动计算字体大小
                                    var pointsize = width / text.length
                                    if (pointsize >= (height * 0.7))
                                        pointsize = height * 0.7
                                    pointsize = pointsize <= 0 ? 1 : pointsize
                                    return pointsize
                                }
                                z: 1
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.IBeamCursor
                                    onClicked: {
                                        setText.connect(setting)
                                        callKeyboard(parent.text)
                                    }
                                    function setting(data, result) {
                                        if (result) {
                                            globalAccess.userinfo.pwd = data
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
                    height: parent.height / 4
                    width: parent.width
                    anchors.left: parent.left
                    anchors.top: pwd_area.bottom
                    Rectangle {
                        id: network_panel
                        anchors.fill: parent
                        anchors.margins: 2
                        color: network_area.enabled ? "#0dffc7" : "#A0A0A0"
                        Behavior on color {
                            ColorAnimation {
                                duration: 500
                            }
                        }
                        Behavior on border.color {
                            ColorAnimation {
                                duration: 500
                            }
                        }
                        Behavior on border.width {
                            NumberAnimation {
                                duration: 500
                            }
                        }
                        border.width: 1
                        border.color: "transparent"
                        radius: 50
                        clip: true
                        Label {
                            anchors.leftMargin: parent.height / 2
                            id: network_title
                            anchors.left: parent.left
                            height: parent.height
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: qsTr("[IP:Port] : ")
                            width: contentWidth
                            font.pointSize: getsize()
                            function getsize() {
                                //自动计算字体大小
                                var pointsize = height
                                if (pointsize >= (height * 0.4))
                                    pointsize = height * 0.4
                                pointsize = pointsize <= 0 ? 1 : pointsize
                                return pointsize
                            }
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
                                text: globalAccess.mainIP.split(
                                          ":").length > 1 ? globalAccess.mainIP.split(
                                                                ":")[0] : globalAccess.mainIP
                                color: "#808080"
                                Behavior on color {
                                    ColorAnimation {
                                        duration: 500
                                    }
                                }
                                //							font.pointSize: height / 3.5
                                font.pointSize: getsize()
                                function getsize() {
                                    //自动计算字体大小
                                    var pointsize = width / text.length
                                    if (pointsize >= (height * 0.7))
                                        pointsize = height * 0.7
                                    pointsize = pointsize <= 0 ? 1 : pointsize
                                    return pointsize
                                }
                                width: parent.width * 0.7
                                z: 1
                                function isValidIP(ip) {
                                    if (ip.length === 0)
                                        return false
                                    var reg = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
                                    return reg.test(ip)
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.IBeamCursor
                                    onClicked: {
                                        setText.connect(setting)
                                        callKeyboard(parent.text)
                                    }
                                    function setting(data, result) {
                                        if (result) {
                                            if (ip_input.isValidIP(data)) {
                                                if (data !== ip_input.text)
                                                    loginBtn_area.state = "detection_Mode"
                                                if (globalAccess.mainIP.split(
                                                            ":").length > 1) {
                                                    globalAccess.mainIP = data + ":"
                                                            + globalAccess.mainIP.split(
                                                                ":")[1]
                                                } else {
                                                    globalAccess.mainIP = data
                                                }
                                            } else
                                                root.reminderTip(
                                                            qsTr("Invalid IP input!"))
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
                                width: contentWidth
                                opacity: port_input.text.length < 1 ? 0 : 1
                                font.pointSize: ip_input.font.pointSize
                            }
                            //Port
                            TextEdit {
                                id: port_input
                                anchors.right: parent.right
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                height: parent.height
                                color: "#808080"
                                text: globalAccess.mainIP.split(
                                          ":").length > 1 ? globalAccess.mainIP.split(
                                                                ":")[1] : "80"
                                width: contentWidth
                                z: 1
                                font.pointSize: getsize()
                                function getsize() {
                                    //自动计算字体大小
                                    var pointsize = height
                                    if (pointsize >= (height * 0.7))
                                        pointsize = height * 0.7
                                    pointsize = pointsize <= 0 ? 1 : pointsize
                                    return pointsize
                                }
                                function isPort(port) {
                                    if (port.length === 0)
                                        return false
                                    var reg = /^(\d*)$/
                                    return reg.test(port)
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.IBeamCursor
                                    onClicked: {
                                        setText.connect(setting)
                                        callKeyboard(parent.text)
                                    }
                                    function setting(data, result) {
                                        if (result) {
                                            if (port_input.isPort(data)) {
                                                if (data !== port_input.text)
                                                    loginBtn_area.state = "detection_Mode"
                                                if (globalAccess.mainIP.split(
                                                            ":").length > 1) {
                                                    globalAccess.mainIP = globalAccess.mainIP.split(
                                                                ":")[0] + ":" + data
                                                } else {
                                                    globalAccess.mainIP += (":" + data)
                                                }
                                            } else
                                                root.reminderTip(
                                                            qsTr("Invalid Network Port input!"))
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
                    height: parent.height / 4
                    width: parent.width / 5 * 3
                    // anchors.right: parent.right
                    // anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.top: network_area.bottom
                    state: "initialize_Mode"
                    states: [
                        State {
                            name: "detection_Mode"
                            PropertyChanges {
                                target: loginBtn
                                enabled: true
                                btnTextContent: qsTr("Detect")
                            }
                            PropertyChanges {
                                target: network_area
                                enabled: true
                            }
                            PropertyChanges {
                                target: uid_area
                                enabled: true
                            }
                            PropertyChanges {
                                target: pwd_area
                                enabled: true
                            }
                        },
                        State {
                            name: "register_Mode"
                            PropertyChanges {
                                target: loginBtn
                                enabled: true
                                btnTextContent: qsTr("Register")
                            }
                            PropertyChanges {
                                target: network_area
                                enabled: true
                            }
                            PropertyChanges {
                                target: uid_area
                                enabled: true
                            }
                            PropertyChanges {
                                target: pwd_area
                                enabled: true
                            }
                        },
                        State {
                            name: "login_Mode"
                            PropertyChanges {
                                target: loginBtn
                                enabled: true
                                btnTextContent: qsTr("Login")
                            }
                            PropertyChanges {
                                target: network_area
                                enabled: true
                            }
                            PropertyChanges {
                                target: uid_area
                                enabled: true
                            }
                            PropertyChanges {
                                target: pwd_area
                                enabled: true
                            }
                        },
                        State {
                            name: "logout_Mode"
                            PropertyChanges {
                                target: loginBtn
                                btnTextContent: qsTr("Logout")
                                enabled: true
                            }
                            PropertyChanges {
                                target: network_area
                                enabled: false
                            }
                            PropertyChanges {
                                target: uid_area
                                enabled: false
                            }
                            PropertyChanges {
                                target: pwd_area
                                enabled: false
                            }
                        },
                        State {
                            name: "admins_Mode"
                            PropertyChanges {
                                target: loginBtn
                                enabled: false
                                btnTextContent: qsTr("Enjoy~")
                            }
                            PropertyChanges {
                                target: network_area
                                enabled: true
                            }
                            PropertyChanges {
                                target: uid_area
                                enabled: false
                            }
                            PropertyChanges {
                                target: pwd_area
                                enabled: false
                            }
                        },
                        State {
                            name: "busy"
                            PropertyChanges {
                                target: loginBtn
                                enabled: false
                                btnTextContent: qsTr("Busy")
                            }
                            PropertyChanges {
                                target: network_area
                                enabled: false
                            }
                            PropertyChanges {
                                target: uid_area
                                enabled: false
                            }
                            PropertyChanges {
                                target: pwd_area
                                enabled: false
                            }
                        },
                        State {
                            name: "initialize_Mode"
                            PropertyChanges {
                                target: loginBtn
                                enabled: false
                                btnTextContent: qsTr("Initializing")
                            }
                            PropertyChanges {
                                target: network_area
                                enabled: false
                            }
                            PropertyChanges {
                                target: uid_area
                                enabled: false
                            }
                            PropertyChanges {
                                target: pwd_area
                                enabled: false
                            }
                        }
                    ]
                    AllBtnStyle {
                        id: loginBtn
                        enabled: true
                        //                        btnTextContent: qsTr("Detect")
                        btnImagePath: ""
                        anchors.fill: parent
                        anchors.margins: 2
                        btnColor: "#8efc60"
                        radius: 10
                        onKeySignal: {
                            if (loginBtn_area.state === "detection_Mode") {
                                loginBtn_area.state = "busy"
                                userLoginModule.sendDetermineLoginStatus()
                            }
                            if (loginBtn_area.state === "register_Mode") {
                                if (pwd_input.text.length >= 6
                                        && uid_input.text.length >= 5) {
                                    loginBtn_area.state = "busy"
                                    userLoginModule.sendUserWizard()
                                } else {
                                    root.reminderTip(
                                                qsTr("Insufficient username or password length was entered!"))
                                }
                            }
                            if (loginBtn_area.state === "login_Mode") {
                                loginBtn_area.state = "busy"
                                userLoginModule.sendUserLogin()
                            }
                            if (loginBtn_area.state === "logout_Mode") {
                                loginBtn_area.state = "busy"
                                userLoginModule.sendUserLogout()
                            }
                        }
                    }
                }
                ///Btn Cancel
                // Rectangle {
                // 	id: cancelBtn_area
                // 	color: "transparent"
                // 	height: parent.height / 3
                // 	width: parent.width / 4
                // 	anchors.right: parent.right
                // 	anchors.top: loginBtn_area.bottom
                // 	AllBtnStyle {
                // 		btnTextContent: qsTr("Cancel")
                // 		btnImagePath: ""
                // 		anchors.fill: parent
                // 		anchors.margins: 2
                // 		btnColor: "#fc6060"
                // 		radius: 10
                // 		onKeySignal: {

                // 		}
                // 	}
                // }
                ///Autologin
                Rectangle {
                    id: autologin_area
                    color: "transparent"
                    height: loginBtn_area.height / 2
                    width: parent.width / 5 * 2
                    anchors.right: parent.right
                    anchors.bottom: remember_area.top
                    FlatSwitch {
                        id: autologin_check
                        anchors.fill: parent
                        text: qsTr("AutoLogin")
                        checked: userLoginModule.autoLogin
                        checkedColor: "#96fed3"
                        onCheckedChanged: {
                            userLoginModule.autoLogin = checked
                            checked = Qt.binding(function () {
                                return userLoginModule.autoLogin
                            })
                            if (autologin_check.checked) {
                                remember_check.checked = checked
                            }
                        }
                    }
                }
                ///Remember me
                Rectangle {
                    id: remember_area
                    color: "transparent"
                    height: loginBtn_area.height / 2
                    width: parent.width / 5 * 2
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    FlatSwitch {
                        id: remember_check
                        anchors.fill: parent
                        text: qsTr("Remember")
                        enabled: !autologin_check.checked
                        checked: userLoginModule.rememberMeBool
                        checkedColor: "#96fed3"
                        onCheckedChanged: {
                            userLoginModule.rememberMeBool = checked
                            checked = Qt.binding(function () {
                                return userLoginModule.rememberMeBool
                            })
                        }
                    }
                }
            }
        }
    }
}
