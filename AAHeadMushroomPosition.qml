import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("AA_X")
        }
        TextField {
            text: aaHeadMushroomPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadMushroomPosition.setX(text)
            }
        }
        Label {
            text: qsTr("AA_Y")
        }
        TextField {
            text: aaHeadMushroomPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadMushroomPosition.setY(text)
            }
        }
        Label {
            text: qsTr("AA_Z")
        }
        TextField {
            text: aaHeadMushroomPosition.Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadMushroomPosition.setZ(text)
            }
        }
    }

    RowLayout {
        Label {
            text: qsTr("AA_A")
        }
        TextField {
            text: aaHeadMushroomPosition.A
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadMushroomPosition.setA(text)
            }
        }
        Label {
            text: qsTr("AA_B")
        }
        TextField {
            text: aaHeadMushroomPosition.B
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadMushroomPosition.setB(text)
            }
        }
        Label {
            text: qsTr("AA_C")
        }
        TextField {
            text: aaHeadMushroomPosition.C
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadMushroomPosition.setC(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.aaMoveToMushroomPos()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorYName)
                var z = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorZName)
                var a = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorAName)
                var b = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorBName)
                var c = baseModuleManager.getMotorFeedbackPos(aaHeadParams.motorCName)

                aaHeadMushroomPosition.setX(x)
                aaHeadMushroomPosition.setY(y)
                aaHeadMushroomPosition.setZ(z)
                aaHeadMushroomPosition.setA(a)
                aaHeadMushroomPosition.setB(b)
                aaHeadMushroomPosition.setC(c)
            }
        }
    }
}
