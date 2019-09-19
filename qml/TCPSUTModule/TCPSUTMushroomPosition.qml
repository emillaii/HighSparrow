import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT_X")
        }
        TextField {
            text: tcpSUTMushroomPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTMushroomPosition.setX(text)
            }
        }
        Label {
            text: qsTr("SUT_Y")
        }
        TextField {
            text: tcpSUTMushroomPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTMushroomPosition.setY(text)
            }
        }
        Label {
            text: qsTr("SUT_Z")
        }
        TextField {
            text: tcpSUTMushroomPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tcpSUTMushroomPosition.setZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {/*
                console.log("sut move to mushroom pos")
                //logicManager.sutMoveToMushroomPos()
                sutModule.moveToMushroomPos()*/
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
//                var x = baseModuleManager.getMotorFeedbackPos(sutParams.motorXName)
//                var y = baseModuleManager.getMotorFeedbackPos(sutParams.motorYName)
//                var z = baseModuleManager.getMotorFeedbackPos(sutParams.motorZName)
//                sutMushroomPosition.setX(x)
//                sutMushroomPosition.setY(y)
//                sutMushroomPosition.setZ(z)
            }
        }
    }
}
