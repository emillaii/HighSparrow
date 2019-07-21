import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
            Label {
                text: qsTr("Dispense_Z")
            }
            TextField {
                text: dispenseParams.dispenseZPos
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator {
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                    dispenseParams.setDispenseZPos(text)
                }
            }

        RowLayout {
            Button {
                text: qsTr("测高")
                width: 20
                height: 40
                onClicked: {
                    dispenseModule.moveToDispenseDot()
                }
            }
        }
    }
}
