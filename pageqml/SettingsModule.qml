import QtQuick 2.9
import "../pageqml"
import "../moduleqml"
import "../"

Item {
	/*普通提醒*/
	signal tip(var outputText)
	/*强提醒*/
	signal reminderTip(var outputText)
}
