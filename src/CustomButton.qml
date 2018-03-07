import QtQuick 2.0
import QtQuick.Controls 2.3
ToolButton {
    id: control
    text: qsTr("ToolButton")
    width: 120

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#000000" : "#161616"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        anchors.fill: parent
        implicitWidth: 40
        implicitHeight: 40
        color: Qt.darker("#33333333", control.enabled && (control.checked || control.highlighted) ? 1.5 : 1.0)
        opacity: enabled ? 1 : 0.3
        visible: control.down || (control.enabled && (control.checked || control.highlighted))
    }
}
