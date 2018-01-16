import QtQuick 2.6
import QtQuick.Controls 2.1

Slider {
    id: sliderid
    value: 0.5

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
            width: sliderid.visualPosition * parent.width
            height: parent.height
            color: "#21be2b"
            radius: 2
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
}
