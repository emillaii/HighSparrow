import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout{
    RowLayout {
        Label { text: qsTr("AA Up Look PR Position") }
    }
    RowLayout {
        Label { text: qsTr("AA 1 PR Position") }
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
        Label {
            text: qsTr("Z")
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
        Button {
            text: title_move_to
            onClicked: {
            }
        }
    }
    RowLayout {
        Label { text: qsTr("AA 2 PR Position") }
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
        Label {
            text: qsTr("Z")
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
        Button {
            text: title_move_to
            onClicked: {
            }
        }
    }
    RowLayout {
        RoundButton{
            id: aa_uplook_location_lock
            transformOrigin: Item.Center
            display: Button.TextBesideIcon
            icon.width: 30
            icon.height: 30
            icon.source: "../../../icons/unlock.png"
            icon.color: "red"
            onClicked: {
                aa_uplook_location_dial.enabled = !aa_uplook_location_dial.enabled
                if (aa_uplook_location_dial.enabled) {
                    aa_uplook_location_lock.icon.color = "lightGreen"
                } else {
                    aa_uplook_location_lock.icon.color = "red"
                }
            }
        }
        Dial{
            id: aa_uplook_location_dial
            enabled: false
            width: 25
            from: 0
            value: aa_uplook_location.lightBrightness
            to: 255
            stepSize: 1
            Label {
                text: aa_uplook_location.lightBrightness
                color: "white"
                font.pixelSize: Qt.application.font.pixelSize * 3
                anchors.centerIn: parent
            }
            onValueChanged: {
            }
        }
        Button{
            text:qsTr("AA Uplook PR Draw")
            onClicked: {
            }
        }
        TextField{
            color: "#57f529"
            text: aa_uplook_location.prFileName
            font.pixelSize: 14
            width: 600
            Layout.preferredWidth: 600
        }
    }
}
