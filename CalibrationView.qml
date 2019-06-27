import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2



ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        spacing: 0
        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            ListModel{
                id: calibration_model
            }
            ListView{
                model:vl_parameter_list
                height: 200 * count
                delegate: GroupBox{
                    title:locationName
                    ColumnLayout{
                        RowLayout{
                            FileDialog{
                                id:file_dialog
                                title:"选择加载PR文件"
                                selectExisting: true
                                selectFolder: false
                                selectMultiple: false

                                nameFilters: ["avdata文件 (*.avdata)"]
                                onAccepted:{
                                    setPrFileName(fileUrl)
                                    //delegateMap[item].setPrFileName(fileUrl)
                                }
                            }
                            Dial{
                                width: 25
                                from: 0
                                value: lightBrightness
                                to: 255
                                stepSize: 1

                                Label {
                                    text: lightBrightness
                                    color: "white"
                                    font.pixelSize: Qt.application.font.pixelSize * 3
                                    anchors.centerIn: parent
                                }
                                onValueChanged: {
                                    if (calibrationViewSwitch.checked) {
                                        setLightBrightness(value)
                                        baseModuleManager.setLightingBrightness(locationName)
                                    }
                                }
                            }
                            Button{
                                text:qsTr("读取PR文件")
                                onClicked: {
                                    file_dialog.open()
                                }
                            }
                            TextField{
                                color: "#57f529"
                                text: prFileName
                                font.pixelSize: 14
                            }
                        }
                        RowLayout{
                            Button{
                                visible: needCalibration
                                text:qsTr("校正")
                                onClicked:{
                                    logicManager.performCalibration(calibrationName)
                                }
                            }
                            Button{
                                text:qsTr("执行PR")
                                onClicked:{
                                    logicManager.performLocation(locationName)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
