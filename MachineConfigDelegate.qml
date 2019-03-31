import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11

ItemDelegate {
    id: root
    width: parent.width
    checkable: true

    onClicked: ListView.view.currentIndex = index

    contentItem: ColumnLayout {
        spacing: 0

        RowLayout {
            ColumnLayout {
                id: dateColumn

                readonly property date alarmDate: new Date(
                    model.year, model.month - 1, model.day, model.hour, model.minute)

                Label {
                    id: timeLabel
                    font.pixelSize: 20
                    text: dateColumn.alarmDate.toLocaleTimeString(window.locale, Locale.ShortFormat)
                }
                RowLayout {
                    Label {
                        id: dateLabel
                        text: dateColumn.alarmDate.toLocaleDateString(window.locale, Locale.ShortFormat)
                    }
                    Label {
                        id: alarmAbout
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Item {
                Layout.fillWidth: true
            }
            Switch {
                checked: model.activated
                Layout.alignment: Qt.AlignTop
                onClicked: model.activated = checked
            }
        }
        CheckBox {
            id: alarmRepeat
            text: qsTr("Repeat")
            checked: model.repeat
            visible: root.checked
            onToggled: model.repeat = checked
        }

        Flow {
            visible: root.checked && model.repeat
            Layout.fillWidth: true

            Repeater {
                id: dayRepeater
                model: daysToRepeat
                delegate: RoundButton {
                    text: Qt.locale().dayName(model.dayOfWeek, Locale.NarrowFormat)
                    flat: true
                    checked: model.repeat
                    checkable: true
                    Material.background: checked ? Material.accent : "transparent"
                    onToggled: model.repeat = checked
                }
            }
        }

        Flow {
            visible: root.checked
            Layout.fillWidth: true
            Repeater {
                id: motorsPositionRepeater
                model: {
                    if (type == "Motion") return motorModels

                }
                delegate: RowLayout {
                    Label {
                        id: motorsName
                        text: model.name
                    }
                    TextField {
                        id: motorValue
                        text: model.value
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: DoubleValidator {
                            bottom: -100
                            top: 100
                            decimals: 3
                            notation: DoubleValidator.StandardNotation
                        }
                        inputMethodHints: Qt.ImhDigitsOnly
                    }
                }
            }
        }

        TextField {
            id: alarmDescriptionTextField
            placeholderText: qsTr("Enter description here")
            cursorVisible: true
            visible: root.checked
            text: model.label
            onTextEdited: model.label = text
        }
        RowLayout {
            Button {
                id: deleteAlarmButton
                text: qsTr("Move To")
                width: 40
                height: 40
                visible: root.checked
                onClicked: {
                    console.log("Move To");
                }
            }
            Button {
                id: readEncoderButton
                text: qsTr("Read Encoder")
                width: 40
                height: 40
                visible: root.checked
                onClicked: {
                    console.log("Read Encdoer");
                }
            }
        }
    }
}
