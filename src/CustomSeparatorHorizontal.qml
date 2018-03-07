import QtQuick 2.0
import QtQuick.Controls 2.3
ToolSeparator {
    padding: 4
    rightPadding: 2
    leftPadding: 2
    topPadding:  2
    bottomPadding:  2
    orientation: Qt.Horizontal
    width: parent.width
    anchors.horizontalCenter: parent.horizontalCenter
    contentItem: Rectangle {
        implicitWidth: 30
        implicitHeight: 1
        color: "#c3c3c3"
    }
}
