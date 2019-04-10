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
                        id: itemLabel
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Switch {
                id: lutViewSwitch
            }
        }
        ColumnLayout {
            visible: lutViewSwitch.checked
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            GroupBox{
                title: qsTr("Load Position")
                LUTLoadPositionView {}
            }
            GroupBox{
                title: qsTr("Pick Lens Position 1")
                LUTPickPositionView {}
            }
            GroupBox{
                title: qsTr("unPick Lens Position 1")
                LUTUnPickPositionView {}
            }
            GroupBox{
                title: qsTr("Pick Lens Position 2")
                LUTPick2PositionView {}
            }
            GroupBox{
                title: qsTr("UnPick Lens Position 2")
                LUTUnPick2PositionView {}
            }
            GroupBox{
                title: qsTr("PR Position")
                LUTPRPositionView {}
            }
            GroupBox{
                title: qsTr("Safety Position")
                LUTSafetyPositionView {}
            }

            GroupBox{
                title: qsTr("Uplook PR Config")
                ColumnLayout {
                    RowLayout {
                        Label {
                            text: qsTr("Load PR Name")
                        }
                        TextField {
                            id: uplookCameraPR
                            text: lutParams.prName
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
                                uplookCameraPR.text = loadfileDialog.fileUrl
                                lutParams.setPRName(loadfileDialog.fileUrl)
                            }
                        }

                        Button {
                            text: qsTr("Load PR Result")
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
                            value: lutParams.Lighting
                            onValueChanged: {
                                lightingController.setUplookLighting(value)
                                lutParams.setLighting(value)
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
