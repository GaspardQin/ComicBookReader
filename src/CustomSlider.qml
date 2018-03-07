import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
Slider {
    id: sliderid

    background: Rectangle {
        x: sliderid.leftPadding
        y: sliderid.topPadding + sliderid.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 3
        width: sliderid.availableWidth
        height: implicitHeight
        radius: 2
        color: "#bdbebf"

        Rectangle {
            color : "#21be2b"
            radius: 2
            width: sliderid.visualPosition * parent.width
            height: parent.height

        }
    }

    handle: Rectangle {
        x: sliderid.leftPadding + sliderid.visualPosition * (sliderid.availableWidth - width)
        y: sliderid.topPadding + sliderid.availableHeight / 2 - height / 2
        implicitWidth: 10
        implicitHeight: 10
        radius: 5
        color: sliderid.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#bdbebf"
    }
    ToolTip {
        opacity: 0.8
        parent: sliderid.handle
        visible: sliderid.pressed
        text: sliderid.value
    }

}
