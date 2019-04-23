import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
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
                id: calibrationViewSwitch
            }
        }
        ColumnLayout {
            visible: calibrationViewSwitch.checked
            GroupBox {
                title: qsTr("SUT Downlook PR")
                CalibrationDownlookView {
                }
            }GroupBox {
                title: qsTr("SUT ToolDownlook PR")
                CalibrationAA1ToolDownlookView{}
            }GroupBox {
                title: qsTr("LUT ToolUplook PR")
                CalibrationAA1ToolUplookView{}
            }
            GroupBox {
                title: qsTr("LUT Mushroom PR")
                CalibrationAA1MushroomView {
                }
            }
            GroupBox {
                title: qsTr("LUT Uplook PR")
                CalibrationUplookView {
                }
            }
            GroupBox {
                title: qsTr("Chart")
                CalibrationChart {
                }
            }
            GroupBox{
                title:qsTr("料盘Lens视觉")
                CalibrationPrLENSLpaLookView{}
            }
            GroupBox{
                title:qsTr("料盘空位视觉")
                CalibrationPrVACANCYLpaLookView{}
            }
            GroupBox{
                title:qsTr("Lut Lens 视觉")
                CalibrationPrLENSLutLookView{}
            }
        }
    }
}
