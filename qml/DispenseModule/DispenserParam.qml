import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SingleheadLSutModuleLib 1.0

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
            readOnly: userManagement.currentAuthority < 2 //At least engineer authority
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
            readOnly: userManagement.currentAuthority < 2 //At least engineer authority
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
            readOnly: userManagement.currentAuthority < 2 //At least engineer authority
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
            text: qsTr("点胶")
            width: 20
            height: 40
            onClicked: {
                aaNewCore.performHandlingOperation(1)
            }
        }
        Button {
            text: qsTr("Downlook PR")
            width: 20
            height: 40
            onClicked: {
                sh_lsut_Module.performHandling(SingleheadLSutModule.MOVE_TO_PR_POSITION +
                                               SingleheadLSutModule.DOWNLOOK_SENSOR_PR)
            }
        }
    }
    RowLayout{
        Image{
            id: dispensePath
            source: "file:///" + dirPath + "//config//vision_dispense_path//resultImageWithPath.jpg"
            cache: false
            Connections{
                target: highSprrow
                onDisplayDispenseImageInUI: {
                    dispensePath.source = ""
                    dispensePath.source = "file:///" + dirPath + "//config//vision_dispense_path//resultImageWithPath.jpg"
                }
            }
        }
    }
}
