import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import LogicManagerLib 1.1

ColumnLayout {

    RowLayout {
        Label {
            text: qsTr("Open_Offset")
        }
        TextField {
            text: dispenserParams.openOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenserParams.setOpenOffset(text)
            }
        }

        Label {
            text: qsTr("Close_Offset")
        }
        TextField {
            text: dispenserParams.closeOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenserParams.setCloseOffset(text)
            }
        }
    }
    RowLayout {
        Label {
            text: qsTr("Speed")
        }
        TextField {
            text: dispenserParams.maximumSpeed
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                dispenserParams.setMaximumSpeed(text)
                dispenserParams.setEndSpeed(text)
            }
        }
        Button {
            text: qsTr("应用速度")
            width: 20
            height: 40
            onClicked: {
                dispenseModule.updateSpeed()
            }
        }
        Button {
            text: qsTr("更新图纸")
            width: 20
            height: 40
            onClicked: {
                dispenseModule.updatePath()
            }
        }
        Button {
            text: qsTr("加载")
            width: 20
            height: 40
            onClicked: {
                baseModuleManager.loadDispenseParam();
            }
        }
        Button {
            text: qsTr("点胶")
            width: 20
            height: 40
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName,1)
            }
        }
    }
    RowLayout {
        Label {
            text: qsTr("UV时间(Ms)")
        }
        TextField {
            text: dispenseParams.uvTimeMs
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                dispenseParams.setUvTimeMs(text)
            }
        }
        Button {
            text: qsTr("去UV")
            width: 20
            height: 40
            onClicked: {
                logicManager.performHandling("", LogicManager.PERFORM_UV)
            }
        }

        DispenseSpeedPopup{
            id: dispenseSpeedPopup
        }

        Button {
            text:qsTr("画胶速度参数")
            onClicked: dispenseSpeedPopup.open()
        }
    }
    RowLayout{
        Rectangle{
            id:showImg;
            width:400;
            height:400;

            Image{
                id:dispensePath
                source: "file:///" + dirPath + "//config//vision_dispense_path//resultImageWithPath.jpg"
                cache: false
                width: parent.width
                height: parent.height
                fillMode: Image.PreserveAspectFit
                Connections{
                    target: highSprrow
                    onDisplayDispenseImageInUI: {
                        dispensePath.source = ""
                        dispensePath.source = "file:///" + dirPath + "//config//vision_dispense_path//resultImageWithPath.jpg"
                    }
                }
            }

            MouseArea{
                 id: showImgMouseArea
                 anchors.fill: showImg
                 //设置拖拽对象以及拖拽区域
                 drag.target: showImg
                 drag.axis: Drag.XAndYAxis//设置横向纵向拖动

                 //设置鼠标悬停以及鼠标响应
                 hoverEnabled: true

                 onDoubleClicked: {
                     console.log("image double clicked")
                     dispensePath.update()
                 }

                 // 鼠标滚轮处理函数
                 onWheel: {
                     if(wheel.angleDelta.y>0&&scaleLevel<=10){//图像放大处理
                         showImg.transformOriginPoint.x = wheel.x
                         showImg.transformOriginPoint.y = wheel.y

                         var beforeWidth  = showImg.width
                         var beforeHeight = showImg.height
                         showImg.width = showImg.width   * scaleValue
                         showImg.height = showImg.height * scaleValue
                         showImgMouseArea.width = showImg.width
                         showImgMouseArea.height = showImg.height
                         showImg.x = showImg.x + wheel.x - showImg.width  * wheel.x / beforeWidth
                         showImg.y = showImg.y + wheel.y - showImg.height * wheel.y / beforeHeight
                         scaleLevel++
                     }
                     else if(wheel.angleDelta.y<0&&scaleLevel>=-10){//图像缩小处理
                         showImg.transformOriginPoint.x = wheel.x
                         showImg.transformOriginPoint.y = wheel.y

                         var beforeWidth  = showImg.width
                         var beforeHeight = showImg.height
                         showImg.width = showImg.width   / scaleValue
                         showImg.height = showImg.height / scaleValue
                         showImgMouseArea.width = showImg.width
                         showImgMouseArea.height = showImg.height
                         showImg.x = showImg.x + wheel.x - showImg.width  * wheel.x / beforeWidth
                         showImg.y = showImg.y + wheel.y - showImg.height * wheel.y / beforeHeight
                         scaleLevel--
                     }
                 }
             }
        }
    }
}
