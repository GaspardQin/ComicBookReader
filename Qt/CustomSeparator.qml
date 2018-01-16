import QtQuick 2.0
import QtQuick.Controls 2.3
ToolSeparator {
    padding: 4
    rightPadding: 2
    leftPadding: 2
    topPadding: vertical ? 2 : 10
    bottomPadding: vertical ? 2 : 10

    contentItem: Rectangle {
        implicitWidth: parent.vertical ? 1 : 24
        implicitHeight: parent.vertical ? 24 : 1
        color: "#c3c3c3"
    }
}
