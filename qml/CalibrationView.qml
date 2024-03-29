﻿import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1


ItemDelegate {
    width: 700
    contentItem: ColumnLayout {
        spacing: 0
        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            ListModel{
                id: calibration_model
            }
            ListView{
                model:vl_parameter_list
                height: 300 * count
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
                                width: 300
                                Layout.preferredWidth: 300
                            }
                        }

                        RowLayout{
                            Label {
                                text: qsTr("PR识别度")
                            }
                            TextField{
                                color: "#57f529"
                                text: objectScore
                                font.pixelSize: 14
                                width: 50
                                Layout.preferredWidth: 50
                                onEditingFinished: {
                                    if (text < 0.2) {
                                        text = 0.2
                                    }
                                    else if (text > 0.98) {
                                        text = 0.98
                                    }
                                    setObjectScore(text)
                                }
                            }
                            CheckBox {
                                text: qsTr("搜索排气孔")
                                visible: displaySmallHoleDetectionSetting
                                checked: enableSmallHoleDetection
                                onCheckedChanged: {
                                    setEnableSmallHoleDetection(checked)
                                }
                            }
                            Label {
                                text: qsTr("小物件 PR识别度")
                                visible: displaySmallHoleDetectionSetting
                            }
                            TextField{
                                visible: displaySmallHoleDetectionSetting
                                color: "#57f529"
                                text: smallObjectScore
                                font.pixelSize: 14
                                width: 50
                                Layout.preferredWidth: 50
                                onEditingFinished: {
                                    if (text < 0.2) {
                                        text = 0.2
                                    }
                                    else if (text > 0.98) {
                                        text = 0.98
                                    }
                                    setSmallObjectScore(text)
                                }
                            }
                        }

                        RowLayout{
                            Button{
                                visible: needCalibration
                                text:qsTr("校正")
                                onClicked:{
                                    //logicManager.performCalibration(calibrationName)
                                    logicManagerState.setCurrentCalibrationName(calibrationName)
                                    logicManager.performHandling(LogicManager.PERFORM_CALIBRATION)
                                }
                            }
                            Button{
                                text:qsTr("执行PR")
                                onClicked:{
                                    //logicManager.performLocation(locationName,true)
                                    logicManagerState.setCurrentLocationName(locationName)
                                    logicManagerState.setUseOriginPr(true)
                                    logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                                }
                            }
                            Button{
                                text:qsTr("执行PR偏移")
                                onClicked:{
                                    //logicManager.performLocation(locationName,false)
                                    logicManagerState.setCurrentLocationName(locationName)
                                    logicManagerState.setUseOriginPr(false)
                                    logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                                }
                            }
                            RoundButton{
                                transformOrigin: Item.Center
                                display: Button.TextBesideIcon
                                icon.width: 30
                                icon.height: 30
                                icon.source: "../../../icons/refresh.png"
                                icon.color: "cyan"
                                onClicked: {
                                    image.source = ""
                                    image.source = prFileName.replace(".avdata", "_resultImage.jpg")
                                }
                            }
                            Image{
                                id: image
                                cache: false
                                sourceSize.width: 100
                                sourceSize.height: 100
                                fillMode: Image.PreserveAspectFit
                                source: {
                                    console.log(prFileName)
                                    var imageName = prFileName.replace(".avdata", "_resultImage.jpg")
                                    return imageName
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
