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
        property double factor: 1.25

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

                }
                else if( wheel.angleDelta.y < 0 ){                        // zoom out
                    showImage.width /= factor
                    showImage.height /= factor
                    showImage.x = oldImgX + (1 - 1/factor)*(mouseX - oldImgX)
                    showImage.y = oldImgY + (1 - 1/factor)*(mouseY - oldImgY)

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
            function zoom(p){
                var oldImgX;
                var oldImgY;
                oldImgX = showImage.x;
                oldImgY = showImage.y;
                showImage.width *= p;
                showImage.height *= p;
                showImage.x = showImage.parent.width/2.0 - p*(showImage.parent.width/2.0 - oldImgX);
                showImage.y = showImage.parent.height/2.0 - p*(showImage.parent.height/2.0 - oldImgY);
            }


            Connections {
                target: buttonZoomIn
                onClicked: showImage.zoom(1.25)
            }
            Connections{
                target: buttonZoomOut
                onClicked: showImage.zoom(0.8)
            }
            Connections{
                target: buttonAutoFit
                onClicked:{
                    showImage.width = showImage.parent.width;
                    showImage.height = showImage.parent.height;
                    showImage.anchors.horizontalCenter = mouseArea.horizontalCenter
                    showImage.anchors.verticalCenter = mouseArea.verticalCenter
                }
            }
        }
        onWidthChanged: {
            showImage.anchors.horizontalCenter = mouseArea.horizontalCenter
            showImage.anchors.verticalCenter = mouseArea.verticalCenter
        }
        onHeightChanged: {
            showImage.anchors.horizontalCenter = mouseArea.horizontalCenter
            showImage.anchors.verticalCenter = mouseArea.verticalCenter
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
                id: buttonZoomIn
                text: qsTr("ZoomIn")
            }
            CustomSeparator{}
            CustomButton {
                id: buttonZoomOut
                text: qsTr("ZoomOut")
            }
            CustomSeparator{}
            CustomButton {
                id: buttonAutoFit
                text: qsTr("AutoFit")
            }
            CustomSeparator{}
            CustomButton {
                id: buttonModeSwitch
                text: qsTr("Mode")//text or image
                onClicked: modePopup.open()
                Popup{
                    id: modePopup
                    width:60
                    height:150
                    x: parent.width/2 - width/2
                    y: -height-5
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                    padding: 0
                    ColumnLayout{
                        anchors.fill: parent
                        CustomButton{
                            id:buttonTextMode
                            text:qsTr("TextMode")
                            height: parent.height/5
                            width: parent.width
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        CustomSeparatorHorizontal{}
                        CustomButton{
                            id:buttonImageMode
                            text:qsTr("ImageMode")
                            height: parent.height/5
                            width: parent.width
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        CustomSeparatorHorizontal{}
                        CustomButton{
                            id:buttonRawMode
                            text: qsTr("RawMode")
                            height: parent.height/5
                            width: parent.width
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
            }
            CustomSeparator{}
            CustomButton {
                id: buttonTwoPageViewSwitch
                text: qsTr("Two") //two pages view
            }


        }
    }

}
