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
        enabled: true
        property double factor: 2.0

        property double oldImgX: 0.0
        property double oldImgY: 0.0
        hoverEnabled: true
        onWheel:{
            oldImgX = showImage.x
            oldImgY = showImage.y
            if(wheel.modifiers & Qt.ControlModifier){
                if( wheel.angleDelta.y > 0 )  // zoom in
                {
                    showImage.width *= factor
                    showImage.height *= factor
                    showImage.x = oldImgX - (factor - 1)*(mouseX - oldImgX)
                    showImage.y = oldImgY - (factor -1)*(mouseY - oldImgY)
                    scrollSlider.value *= factor
                }
                else if( wheel.angleDelta.y < 0 ){                        // zoom out
                    showImage.width /= factor
                    showImage.height /= factor
                    showImage.x = oldImgX + (1 - 1/factor)*(mouseX - oldImgX)
                    showImage.y = oldImgY + (1 - 1/factor)*(mouseY - oldImgY)
                    scrollSlider.value /= factor
                }

            }


        }


        Image {
            id: showImage
            width: parent.width
            height: parent.height
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.verticalCenter: parent.verticalCenter
            property int pageNum: 1
            property int showMode: 0 //0 for img, 1 for text

            source: "image://ImageProvider/" + pageNum.toString() + "/" + showMode.toString()
            cache: false
            fillMode: Image.PreserveAspectFit
            mipmap: true

            Drag.active: mouseArea.drag.active

            states: State {
                    when: mouseArea.drag.active

                    AnchorChanges { target: showImage; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
            }
            onPageNumChanged: {
                if(pageNum > slider.to){
                    pageNum = slider.to;
                }else if(pageNum < 1){
                    pageNum = 1;
                }

                anchors.horizontalCenter = parent.horizontalCenter
                anchors.verticalCenter = parent.verticalCenter
            }
            Connections {
                target: scrollSlider
                property double oldImgX
                property double oldImgY
                onMoved:{
                    oldImgX = showImage.x
                    oldImgY = showImage.y
                    showImage.width = showImage.parent.width * scrollSlider.value
                    showImage.height = showImage.parent.height * scrollSlider.value
                    showImage.x = showImage.parent.width/2.0 - scrollSlider.value*(showImage.parent.width/2.0 - oldImgX)
                    showImage.y = showImage.parent.height/2.0 - scrollSlider.value*(showImage.parent.height/2.0 - oldImgY)
                }
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
                    onOpened: {
                        mouseArea.enabled = false
                    }
                    onClosed: {
                        mouseArea.enabled = true
                    }

                    CustomSliderVertical{
                        id: scrollSlider
                        x: parent.width/2 - width/2
                        y: parent.height/2 - height/2
                        height: 100
                        width: 20
                        opacity: 1
                        from:0.25
                        to:5.0
                        value:1.0
                        stepSize: 0.05
                        snapMode: Slider.SnapAlways


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
