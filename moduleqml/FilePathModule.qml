import QtQuick 2.9
import QtQml 2.0
import "../pageqml"
import "../moduleqml"
import "../"

Item {
    property string file_version: "1.0.0.0" //文件版本控制
    width: 800
    height: 480
    id: root
    /*普通提醒*/
    signal tip(var outputText)
    /*强提醒*/
    signal reminderTip(var outputText)
    property double progress: 0
    property int fileId: 0 //file List Id(Global Id)
    property int step: 0 //0-no 1-print 2-slice 3-upload
    property int step_state: 0 //0-ready 1-work 2-failure
    property int storage: 0 //0-local (0>)-external
    property string fileName: "" //display
    property string fileType: ""
    property string fileURL: "" //refs_download
    property string fileResource: "" //refs_resource
    property string localPath: ""
    property double estimatedPrintTime: 0.0 //gcodeAnalysis_estimatedPrintTime
    Rectangle {
        anchors.fill: parent
        color: "transparent"
    }
}
