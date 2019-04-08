import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("LUT_X")
        }
        TextField {
            text: lutParams.UnPick2X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutParams.setUnPick2X(text)
            }
        }
        Label {
            text: qsTr("LUT_Y")
        }
        TextField {
            text: lutParams.UnPick2Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                bottom: -100
                top: 100
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lutParams.setUnPick2Y(text)
            }
        }
        Label {
            text: qsTr("LUT_Z")
        }
        TextField {
            text: lutParams.UnPick2Z
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                console.log("Set UnPick 2")
                lutParams.setUnPick2Z(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            visible: root.checked
            onClicked: {
                lutModule.moveToUnPick2Lens()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            visible: root.checked
            onClicked: {
                console.log("Read Encdoer");
            }
        }
    }
}
