import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("ComicBookReader")

    ScrollView {
        id: imageView
        width: parent.width
        clip: true
        anchors.top:parent.top
        anchors.bottom: slider.top
        anchors.left: parent.left
        anchors.right: parent.right
        Layout.fillHeight: true
        Label {
            text: "this is for images"
            font.pixelSize: 224
        }
    }
    CustomSlider {
        id:slider
        anchors.bottom:  bottomBar.top
        width:parent.width

        anchors.bottomMargin: 0
        rightPadding: 3
        bottomPadding: 0
        leftPadding: 3
        topPadding: 0


        from: 1
        value: 25
        to: 100
    }
    ToolBar{
        id: bottomBar
        //position: ToolBar.Footer
        anchors.bottomMargin: 0
        //anchors.topMargin: (parent.height-50)
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 50
        RowLayout {
            anchors.rightMargin: 0
            anchors.fill: parent
            CustomButton {
                text: qsTr("PopUpMenu")
                Layout.fillWidth: false
                autoRepeat: false
                focusPolicy: Qt.WheelFocus
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                onClicked: menu.open();
                Menu {
                    id: menu
                    spacing: 0


                    bottomMargin: bottomBar.height + 10
                   //bottomPadding: 5
                    enter: Transition {
                        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
                    }
                    MenuItem {
                        text: "New..."
                    }
                    MenuItem {
                        text: "Open..."
                    }
                    MenuItem {
                        text: "Save"
                    }
                }





            }
            CustomSeparator{}
            Item {
                id: item1
                Layout.fillWidth: true
                CustomButton {
                    id: buttonPageDown
                    width: parent.width/2.0
                    text: qsTr("PageDown")
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }
                CustomButton {
                    id: buttonPageUp
                    text: qsTr("PageUp")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: buttonPageDown.right
                    anchors.right: parent.right
                }
            }

            CustomSeparator{}
            CustomButton {
                text: qsTr("Scaling")
            }
            CustomSeparator{}
            CustomButton {
                text: qsTr("T/I")//text or image
            }
            CustomSeparator{}
            CustomButton {
                text: qsTr("Two") //two pages view
            }


        }
    }
}
