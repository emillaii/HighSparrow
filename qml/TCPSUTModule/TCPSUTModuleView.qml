import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import "../.."
import LogicManagerLib 1.1

ItemDelegate {
    contentItem: ColumnLayout {
        ListModel{
            id:motorsModel
            ListElement{
                modelData:"SUT_X"
            }
            ListElement{
                modelData:"SUT_Y"
            }
            ListElement{
                modelData:"SUT_Z"
            }
        }

        MotionPopup{
            id:sut_model_motions
        }

        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("")
                }
                Button{
                    text:qsTr("手动操作")
                    onClicked:{
                        sut_model_motions.open()
                    }
                }
                Button{
                    text:qsTr("加载")
                    onClicked:{
                        baseModuleManager.loadModuleParameter(sutParams.moduleName)
                    }
                }
            }


            GroupBox{
                title: qsTr("Mushroom Position")
                TCPSUTMushroomPosition {}
            }
            GroupBox{
                title: qsTr("Load Position")
                TCPSUTLoadPositionView {}
            }
            GroupBox{
                title: qsTr("PR Position")
                TCPSUTPRPositionView {}
            }
            GroupBox{
                title: qsTr("UpDn DownLook Calibration Position")
                TCPSUTToollookPosition {}
            }
            GroupBox{
                title: qsTr("UpDn UpLook Calibration Position")
                TCPSUTToolUplookPosition {}
            }
            GroupBox{
                title: qsTr("Safety Position")
                TCPSUTSafetyPositionView {}
            }
            RowLayout {
                GroupBox {
                    title: qsTr("Updownlook Camera Offset")
                    ColumnLayout {
                        RowLayout {
                            Button{
                                text:qsTr("UpDn Calib")
                                onClicked: {
                                    logicManager.performHandling(LogicManager.PERFORM_UPDNLOOK_CALIBRATION)
                                }
                            }
                            Label {
                                text: qsTr("Camera Offset")
                            }
                            TextField {
                                text: tcpSUTParams.cameraTheta
                                horizontalAlignment: TextInput.AlignHCenter
                                validator: DoubleValidator {
                                    decimals: 6
                                    notation: DoubleValidator.StandardNotation
                                }
                                onEditingFinished: {
                                    tcpSUTParams.setCameraTheta(text)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
