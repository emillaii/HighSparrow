import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import SomeLib 1.1
import LogicManagerLib 1.1
Column{
    RowLayout {
        Label {
            text: qsTr("料盘1")
        }
        Label {
            text: qsTr("Row")
        }
        TextField {
            text: "1"
            id: t_nrow_1
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
            }
        }
        Label {
            text: qsTr("Col")
        }
        TextField {
            text: "1"
            id: t_ncol_1
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayCurrent(t_ncol_1.text-1,t_nrow_1.text-1,0)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS)
            }
        }
    }
    RowLayout {
        Label {
            text: qsTr("料盘2")
        }
        Label {
            text: qsTr("Row")
        }
        TextField {
            text: "1"
            id: t_nrow_2
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                material_tray.setTrayCurrent(t_ncol_2.text-1,t_nrow_2.text-1,1)
            }
        }
        Label {
            text: qsTr("Col")
        }
        TextField {
            text: "1"
            id: t_ncol_2
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                material_tray.setTrayCurrent(t_ncol_2.text-1,t_nrow_2.text-1,1)
            }
        }
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayCurrent(t_ncol_2.text-1,t_nrow_2.text-1,1)
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS)
            }
        }
    }

}
