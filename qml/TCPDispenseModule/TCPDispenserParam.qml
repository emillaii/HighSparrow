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
            text: tcpDispenserParams.openOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenserParams.setOpenOffset(text)
            }
        }

        Label {
            text: qsTr("Close_Offset")
        }
        TextField {
            text: tcpDispenserParams.closeOffset
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenserParams.setCloseOffset(text)
            }
        }
    }
    RowLayout {
        Label {
            text: qsTr("Speed")
        }
        TextField {
            text: tcpDispenserParams.maximumSpeed
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpDispenserParams.setMaximumSpeed(text)
                tcpDispenserParams.setEndSpeed(text)
            }
        }
        Button {
            text: qsTr("更新图纸")
            width: 20
            height: 40
            onClicked: {
                //dispenseModule.updatePath()
            }
        }
        Button {
            text: qsTr("加载")
            width: 20
            height: 40
            onClicked: {
                //baseModuleManager.loadDispenseParam();
            }
        }
        Button {
            text: qsTr("点胶")
            width: 20
            height: 40
            onClicked: {
                //aaNewCore.performHandlingOperation(1,"")
            }
        }
    }
    RowLayout {
        Label {
            text: qsTr("UV时间(Ms)")
        }
        TextField {
            text: tcpDispenserParams.uvTimeMs
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                tcpDispenserParams.setUvTimeMs(text)
            }
        }
        Button {
            text: qsTr("去UV")
            width: 20
            height: 40
            onClicked: {
                //logicManager.performHandling("", LogicManager.PERFORM_UV)
            }
        }
    }
    RowLayout{
        Image{
            source: "file:///" + dirPath + "//config//vision_dispense_path//resultImageWithPath.jpg"
        }
    }
}
