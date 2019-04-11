import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2


ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        RowLayout {
            ColumnLayout {
                RowLayout {
                    Label {
                        id: itemLabel
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Switch {
                id: sutViewSwitch
            }
        }
        ColumnLayout {
            visible: sutViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            GroupBox{
                title: qsTr("OC Position")
                SUTOCPositionView {}
            }
            GroupBox{
                title: qsTr("Load Position")
                SUTLoadPositionView {}
            }
            GroupBox{
                title: qsTr("PR Position")
                SUTPRPositionView {}
            }
            GroupBox{
                title: qsTr("Safety Position")
                SUTSafetyPositionView {}
            }

            GroupBox{
                title: qsTr("Uplook PR Config")
                ColumnLayout {
                    RowLayout {
                        Label {
                            text: qsTr("选择加载PR文件")
                        }
                        TextField {
                            id: downlookCameraPR
                            text: sutParams.prName
                            horizontalAlignment: TextInput.AlignHCenter
                            onTextChanged: {
                            }
                        }
                        FileDialog {
                            id: loadfileDialog
                            title: "选择加载PR文件"
                            selectExisting: true
                            selectFolder: false
                            selectMultiple: false

                            nameFilters: ["avdata文件 (*.avdata)"]
                            onAccepted: {
                                downlookCameraPR.text = loadfileDialog.fileUrl
                                sutParams.setPRName(loadfileDialog.fileUrl)
                            }
                        }

                        Button {
                            text: qsTr("选择加载PR文件")
                            width: 20
                            height: 40
                            onClicked: {
                                loadfileDialog.open()
                            }
                        }
                        Slider {
                            id: slider
                            stepSize: 1
                            to: 255
                            from: 0
                            value: sutParams.Lighting
                            onValueChanged: {
                                lightingController.setDownlookLighting(value)
                                sutParams.setLighting(value)
                            }
                        }
                        Label {
                            y: 6
                            color: "#46eb46"
                            text: slider.value
                            font.pointSize: 20
                            font.family: "Times New Roman"
                        }
                    }
                }
            }
        }
    }
}
