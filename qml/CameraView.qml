import QtQuick 2.9
import QtQuick.Controls 2.2

Grid {
    id: grid
    columns: 2
    width: 800
    height: 800

    function calculateOffset(w, h, px, py, rw, rh)
    {
        var delta = {};
        delta.x = (w/2 - px)*rw;
        delta.y = (h/2 - py)*rh;
        return delta;
    }

    Frame {
        id: frame3
        width: grid.width/2
        height: grid.height/2
        Image {
            id: image3
            anchors.fill: parent
            source: "../icons/sparrow.png"
            fillMode: Image.PreserveAspectFit
            cache: false

            Rectangle {
                color: "pink"
                opacity: 0.8
                x: (image3.width - image3.paintedWidth)/2
                y: image3.height/2
                width: image3.paintedWidth
                height: 1
            }
            Rectangle {
                color: "pink"
                opacity: 0.8
                x: image3.width/2
                y: (image3.height - image3.paintedHeight)/2
                width: 1
                height: image3.paintedHeight
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: "CrossCursor"
                onDoubleClicked: {
                    var borderX = (image3.width - image3.paintedWidth) / 2
                    var borderY = (image3.height - image3.paintedHeight) / 2
                    if (mouseX < borderX || mouseX > image3.width - borderX ||
                        mouseY < borderY || mouseY > image3.height - borderY)
                    {
                        return console.log("Image Click Out of range")
                    }
                    console.log("image.sourceSize.width: " + image3.sourceSize.width + " image.sourceSize.height: " + image3.sourceSize.height)
                    var delta = calculateOffset(image3.width, image3.height, mouseX, mouseY,
                                                image3.sourceSize.width/image3.paintedWidth,
                                                image3.sourceSize.height/image3.paintedHeight)
                    console.log("delta x: " + delta.x +  ", delta y:" + delta.y)
                }
            }
            Text {
                id: element
                x: -12
                y: -12
                color: "#9ef678"
                text: qsTr("Image Grabber")
                wrapMode: Text.WordWrap
                elide: Text.ElideLeft
                fontSizeMode: Text.Fit
                lineHeight: 3.1
                anchors.fill: parent
                font.pixelSize: 12
            }
            Slider {
                id: slider3
                y: 250
                width: 120
                height: 48
                stepSize: 1
                to: 255
                from: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -22
                anchors.left: parent.left
                anchors.leftMargin: -5
                value: 0
                onValueChanged: {
                    //lightingController.setPickarmLighting(value)
                }
                Label {
                    y: 10
                    color: "#46eb46"
                    text: slider3.value
                    font.pointSize: 20
                    font.family: "Times New Roman"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 117
                }
                Button {
                    x: 241
                    y: 0
                    width: 50
                    height: 50
                    text: "Button"
                    display: AbstractButton.IconOnly
                    icon.color: "lightGreen"
                    icon.source: "../icons/save.png"
                    onClicked: {
                        console.log("Save Image Grabber Image")
                        imageGrabberThread.saveImage()
                    }
                }
            }
        }
        Connections {
            target: imageGrabberThread
            onCallQmlRefeshImg: {
                image3.source = ""
                image3.source = "image://imageGrabberLiveImage"
            }
        }
    }

    Frame {
        id: frame
        width: grid.width/2
        height: grid.height/2

        Image {
            id: image
            anchors.fill: parent
            source: "../icons/sparrow.png"
            fillMode: Image.PreserveAspectFit
            cache: false

            Rectangle {
                color: "springgreen"
                opacity: 0.8
                x: (image.width - image.paintedWidth)/2
                y: image.height/2
                width: image.paintedWidth
                height: 1
            }
            Rectangle {
                color: "springgreen"
                opacity: 0.8
                x: image.width/2
                y: (image.height - image.paintedHeight)/2
                width: 1
                height: image.paintedHeight
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: "CrossCursor"
                onDoubleClicked: {
                    var borderX = (image.width - image.paintedWidth) / 2
                    var borderY = (image.height - image.paintedHeight) / 2
                    if (mouseX < borderX || mouseX > image.width - borderX ||
                        mouseY < borderY || mouseY > image.height - borderY)
                    {
                        return console.log("Image Click Out of range")
                    }
                    console.log("width=" + image.width + ", hight=" + image.height + ", mouseX=" + mouseX + ", mouseY=" + mouseY)
                    console.log("image.sourceSize.width: " + image.sourceSize.width + " image.sourceSize.height: " + image.sourceSize.height)
                    var delta = calculateOffset(image.width, image.height, mouseX, mouseY,
                                                image.sourceSize.width/image.paintedWidth,
                                                image.sourceSize.height/image.paintedHeight)
                    console.log("delta x: " + delta.x +  ", delta y:" + delta.y)
                    sh_lsut_Module.moveToCamPos(delta.x, delta.y, 0)
                }
            }
            Text {
                x: -12
                y: -12
                color: "#9ef678"
                text: qsTr("Uplook Camera")
                wrapMode: Text.WordWrap
                elide: Text.ElideLeft
                fontSizeMode: Text.Fit
                lineHeight: 3.1
                anchors.fill: parent
                font.pixelSize: 12
            }

            Slider {
                id: slider
                y: 256
                width: 120
                height: 48
                stepSize: 1
                to: 255
                from: 0

                anchors.left: parent.left
                anchors.leftMargin: -3
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -28
                value: lightingController.uplookLighting
                onValueChanged: {
                    lightingController.setUplookLighting(value)
                }
                Label {
                    id: label
                    y: 10
                    color: "#46eb46"
                    text: lightingController.uplookLighting
                    font.pointSize: 20
                    font.family: "Times New Roman"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 115
                }
                Button {
                    x: 241
                    y: 0
                    width: 50
                    height: 50
                    text: "Button"
                    display: AbstractButton.IconOnly
                    icon.color: "lightGreen"
                    icon.source: "../icons/save.png"
                    onClicked: {
                        console.log("Save Image")
                        visionModule.saveImage(0)
                    }
                }
            }

            Connections {
                target: uplookCamera
                onCallQmlRefeshImg: {
                    image.source = ""
                    image.source = "image://uplookCameraImage"
                }
            }
        }
    }

    Frame {
        id: frame1
        width: grid.width/2
        height: grid.height/2

        Image {
            id: image1
            anchors.fill: parent
            source: "../icons/sparrow.png"
            fillMode: Image.PreserveAspectFit
            cache: false

            Rectangle {
                color: "springgreen"
                opacity: 0.8
                x: (image1.width - image1.paintedWidth)/2
                y: image1.height/2
                width: image1.paintedWidth
                height: 1
            }
            Rectangle {
                color: "springgreen"
                opacity: 0.8
                x: image1.width/2
                y: (image1.height - image1.paintedHeight)/2
                width: 1
                height: image1.paintedHeight
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: "CrossCursor"
                onDoubleClicked: {
                    var borderX = (image1.width - image1.paintedWidth) / 2
                    var borderY = (image1.height - image1.paintedHeight) / 2
                    if (mouseX < borderX || mouseX > image1.width - borderX ||
                        mouseY < borderY || mouseY > image1.height - borderY)
                    {
                        return console.log("Image Click Out of range")
                    }
                    console.log("image.sourceSize.width: " + image1.sourceSize.width + " image.sourceSize.height: " + image1.sourceSize.height)
                    console.log("width=" + image1.width + ", hight=" + image1.height + ", mouseX=" + mouseX + ", mouseY=" + mouseY)
                    var delta = calculateOffset(image1.width, image1.height, mouseX, mouseY,
                                                image1.sourceSize.width/image1.paintedWidth,
                                                image1.sourceSize.height/image1.paintedHeight)
                    console.log("delta x: " + delta.x +  ", delta y:" + delta.y)
                    sh_lsut_Module.moveToCamPos(delta.x, delta.y, 1)
                }
            }

            Slider {
                y: 255
                stepSize: 1
                to: 255
                from: 0
                width: 120
                height: 48
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -27
                value: lightingController.downlookLighting
                onValueChanged: {
                    lightingController.setDownlookLighting(value)
                }
                Label {
                    y: 10
                    color: "#46eb46"
                    text: lightingController.downlookLighting
                    font.pointSize: 20
                    font.family: "Times New Roman"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 118
                }
                Button {
                    x: 241
                    y: 0
                    width: 50
                    height: 50
                    text: "Button"
                    display: AbstractButton.IconOnly
                    icon.color: "lightGreen"
                    icon.source: "../icons/save.png"
                    onClicked: {
                        console.log("Save Image")
                        visionModule.saveImage(1)
                    }
                }
            }

            Connections {
                target: downlookCamera
                onCallQmlRefeshImg: {
                    image1.source = ""
                    image1.source = "image://downlookCameraImage"
                }
            }
        }

        Text {
            id: element1
            color: "#9ef678"
            text: qsTr("AA Downlook Camera")
            fontSizeMode: Text.Fit
            lineHeight: 3.1
            anchors.fill: parent
            font.pixelSize: 12
        }
    }


    Frame {
        id: frame2
        width: grid.width/2
        height: grid.height/2
        Image {
            id: image2
            anchors.fill: parent
            source: "../icons/sparrow.png"
            fillMode: Image.PreserveAspectFit
            cache: false
            Rectangle {
                color: "springgreen"
                opacity: 0.8
                x: (image.width - image.paintedWidth)/2
                y: image.height/2
                width: image.paintedWidth
                height: 1
            }
            Rectangle {
                color: "springgreen"
                opacity: 0.8
                x: image.width/2
                y: (image.height - image.paintedHeight)/2
                width: 1
                height: image.paintedHeight
            }

            Connections {
                target: pickarmCamera
                onCallQmlRefeshImg: {
                    image2.source = ""
                    image2.source = "image://pickarmCameraImage"
                }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: "CrossCursor"
                onDoubleClicked: {
                    var borderX = (image2.width - image2.paintedWidth) / 2
                    var borderY = (image2.height - image2.paintedHeight) / 2
                    if (mouseX < borderX || mouseX > image2.width - borderX ||
                        mouseY < borderY || mouseY > image2.height - borderY)
                    {
                        return console.log("Image Click Out of range")
                    }
                    console.log("width=" + image2.width + ", hight=" + image2.height + ", mouseX=" + mouseX + ", mouseY=" + mouseY)
                    console.log("image.sourceSize.width: " + image2.sourceSize.width + " image.sourceSize.height: " + image2.sourceSize.height)
                    var delta = calculateOffset(image2.width, image2.height, mouseX, mouseY,
                                                image2.sourceSize.width/image2.paintedWidth,
                                                image2.sourceSize.height/image2.paintedHeight)
                    console.log("delta x: " + delta.x +  ", delta y:" + delta.y)
                    sh_materialLoaderModule.moveToCamPos(delta.x, delta.y)
                }
            }
            Text {
                x: -12
                y: -12
                color: "#9ef678"
                text: qsTr("Pickarm Camera")
                wrapMode: Text.WordWrap
                elide: Text.ElideLeft
                fontSizeMode: Text.Fit
                lineHeight: 3.1
                anchors.fill: parent
                font.pixelSize: 12
            }
            Slider {
                id: slider1
                y: 250
                width: 120
                height: 48
                stepSize: 1
                to: 255
                from: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -22
                anchors.left: parent.left
                anchors.leftMargin: -5
                value: lightingController.pickarmLighting
                onValueChanged: {
                    lightingController.setPickarmLighting(value)
                }
                Label {
                    y: 10
                    color: "#46eb46"
                    text: lightingController.pickarmLighting
                    font.pointSize: 20
                    font.family: "Times New Roman"
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 117
                }
                Button {
                    x: 241
                    y: 0
                    width: 50
                    height: 50
                    text: "Button"
                    display: AbstractButton.IconOnly
                    icon.color: "lightGreen"
                    icon.source: "../icons/save.png"
                    onClicked: {
                        console.log("Save Image")
                        visionModule.saveImage(2)
                    }
                }
            }

        }
    }

}
