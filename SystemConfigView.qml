import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        spacing: 0
        RowLayout {
            ColumnLayout {
                RowLayout {
                    Label {
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Switch {
                id: systemConfigViewSwitch
            }
        }

        ColumnLayout {
            visible: systemConfigViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            GroupBox{
                ColumnLayout {
                    RowLayout {
                        Label {
                            text: qsTr("Server Mode ")
                        }
                        ComboBox {
                            id: selectedMode
                            width: 200
                            model: [ "Master", "Slave"]
                        }
                    }
                    RowLayout {
                        visible: {
                            return selectedMode.currentIndex == 0
                        }
                        Label {
                            text: qsTr("Server Port ")
                        }
                        TextField {
                            text: baseModuleManager.ServerPort
                            validator: IntValidator {
                            }
                            onEditingFinished: {
                                baseModuleManager.setServerPort()
                            }
                        }
                    }
                    RowLayout {
                        visible: {
                            return selectedMode.currentIndex != 0
                        }
                        Label {
                            text: qsTr("Server URL ")
                        }
                        TextField {
                            text: baseModuleManager.ServerURL
                            width: 300
                            Layout.preferredWidth: 300
                            onEditingFinished: {
                                baseModuleManager.setServerURL()
                            }
                        }
                        Button {
                            text: qsTr("Connect")
                            onClicked: {
                                console.log("connecting...")
                            }
                        }
                    }
                }
            }
        }
    }
}

