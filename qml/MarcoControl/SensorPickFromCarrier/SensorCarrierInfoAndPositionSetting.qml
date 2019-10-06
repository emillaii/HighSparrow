import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
    RowLayout { Label { text: qsTr("Sensor Carrier Info") } }
    RowLayout {
        Label { text: qsTr("料盘列数") }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label { text: qsTr("料盘行数") }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
    }
    RowLayout {
        Label { text: qsTr("料盘列距") }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label { text: qsTr("料盘行距") }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: qsTr("计算")
        }
    }
    RowLayout {
        Label { text: qsTr("料盘1起点") }
        Label {
            text: qsTr("X")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
            }
        }
        Label { text: qsTr("料盘1终点") }
        Label {
            text: qsTr("X")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
            }
        }
    }
    RowLayout {
        Label { text: qsTr("料盘2起点") }
        Label {
            text: qsTr("X")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
            }
        }
    }
    SensorCarrierInfoAndPositionTestOperation {}
}
