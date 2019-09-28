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
            text: qsTr("更新图纸")
            width: 20
            height: 40
            onClicked: {
                //dispensePath.update()
                dispenseModule.updatePath()
                //dispensePath.source = "file:///" + dirPath + "//config//vision_dispense_path//resultImageWithPath.jpg"
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
//                aaNewCore.performHandling(1,"")
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
    }
    RowLayout{
        Image{
            id:dispensePath
            source: "file:///" + dirPath + "//config//vision_dispense_path//resultImageWithPath.jpg"
        }
    }
}
