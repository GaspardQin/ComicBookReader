import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
Slider {
    id: slideridvertical
    orientation: Qt.Vertical
    background: Rectangle {
        id: rectangle1
        //x: sliderid.leftPadding
        //y: sliderid.topPadding + sliderid.availableHeight / 2 - height / 2

        y: slideridvertical.topPadding
        x: slideridvertical.leftPadding + slideridvertical.availableWidth / 2 - width / 2
        implicitWidth: 3
        implicitHeight: 200
        height: slideridvertical.availableHeight
        width: implicitWidth
        radius: 2
        color: "#21be2b"

        Rectangle {
            id: rectangle2
            color : "#bdbebf"
            radius: 2
            height: slideridvertical.visualPosition * parent.height
            width:  parent.width

        }
    }

    handle: Rectangle {
        y: slideridvertical.topPadding + slideridvertical.visualPosition * (slideridvertical.availableHeight - height)
        x: slideridvertical.leftPadding + slideridvertical.availableWidth / 2 - width / 2
        implicitWidth: 10
        implicitHeight: 10
        radius: 7
        color: slideridvertical.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#bdbebf"

    }
    ToolTip {

        parent: slideridvertical.handle
        visible: slideridvertical.pressed
        text: slideridvertical.value.toFixed(2)
    }

}
