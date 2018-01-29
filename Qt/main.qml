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

    MouseArea {
        id: mouseArea
        width: parent.width

        anchors.top:parent.top
        anchors.bottom: slider.top
        anchors.left: parent.left
        anchors.right: parent.right
        Layout.fillHeight: true

        drag.target: showImage

        //DropArea {
       //     anchors.fill: parent
            //onEntered: drag.source.caught = true;
           // onExited: drag.source.caught = false;
        //}


        Image {
            id: showImage
            width: parent.width
            height: parent.height
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.verticalCenter: parent.verticalCenter
            property int pageNum: 1
            property int showMode: 0 //0 for img, 1 for text
            source: "image://ImageProvider/" + pageNum.toString() + "/" + showMode.toString()
            fillMode: Image.PreserveAspectFit
            mipmap: true

            Drag.active: mouseArea.drag.active
            states: State {
                when: mouseArea.drag.active

                AnchorChanges { target: showImage; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
            }

            onPageNumChanged: {
                anchors.horizontalCenter = parent.horizontalCenter
                anchors.verticalCenter = parent.verticalCenter
            }

        }
    }
    CustomSlider {
        id:slider
        anchors.bottom:  bottomBar.top
        width:parent.width
        objectName: "SlideBar"
        anchors.bottomMargin: 0
        rightPadding: 3
        bottomPadding: 0
        leftPadding: 3
        topPadding: 0
        from : 1
        value: 1
        to :1
        snapMode: Slider.SnapOnRelease
        stepSize: 1
        Connections {
            target: showImage

            onPageNumChanged: {
                slider.value = showImage.pageNum
            }
        }
        onValueChanged: {
            showImage.pageNum = slider.value;
        }



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
                    onClicked: showImage.pageNum -=1
                }
                CustomButton {
                    id: buttonPageUp
                    text: qsTr("PageUp")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: buttonPageDown.right
                    anchors.right: parent.right
                    onClicked: showImage.pageNum +=1
                }
            }

            CustomSeparator{}
            CustomButton {
                id: buttonScaling
                text: qsTr("Scaling")
                onClicked: scrollSliderPopup.open()
                Popup {
                    id: scrollSliderPopup

                    width: 20
                    height: 100
                    x: parent.width/2 - width/2
                    y: -height-5
                    focus: true
                    padding: 0
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                    opacity: 0.9
                    CustomSliderVertical{
                        id: scrollSlider
                        x: parent.width/2 - width/2
                        y: parent.height/2 - height/2
                        height: 100
                        width: 20
                        opacity: 1
                    }
                }

            }
            CustomSeparator{}
            CustomButton {
                id: buttonTextImageSwitch
                text: qsTr("T/I")//text or image
            }
            CustomSeparator{}
            CustomButton {
                id: buttonTwoPageViewSwitch
                text: qsTr("Two") //two pages view
            }


        }
    }

}
