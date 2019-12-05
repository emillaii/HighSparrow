import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
    RowLayout { Label { text: qsTr("Sensor Carrier Info") } }
    RowLayout {
        Label { text: qsTr("料盘列数") }
        TextField {
            text: tray_standard_parameter.columnCount
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_standard_parameter.setColumnCount(text)
            }
        }
        Label { text: qsTr("料盘行数") }
        TextField {
            text: tray_standard_parameter.rowCount
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_standard_parameter.setRowCount(text)
            }
        }
    }
    RowLayout {
        Label { text: qsTr("料盘列距") }
        TextField {
            text: tray_standard_parameter.columnDeta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_standard_parameter.setColumnDeta(text)
            }
        }
        Label { text: qsTr("料盘行距") }
        TextField {
            text: tray_standard_parameter.rowDelta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_standard_parameter.setRowDelta(text)
            }
        }
        Button {
            text: qsTr("计算")
            onClicked: {
                material_tray.calculateDelta()
            }
        }
    }
    RowLayout {
        Label { text: qsTr("料盘1起点") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tray_start_point1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point1.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tray_start_point1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point1.setY(text)
            }
        }
        Button {
            text: title_move_to
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_START_POS)
            }
        }

        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                tray_start_point1.setX(x);
                tray_start_point1.setY(y);
            }
        }
        Label { text: qsTr("料盘1终点") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: first_tray_end_position.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                first_tray_end_position.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: first_tray_end_position.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                first_tray_end_position.setY(text)
            }
        }
        Button {
            text: title_move_to
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_END_POS)
            }
        }

        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                first_tray_end_position.setX(x);
                first_tray_end_position.setY(y);
            }
        }
    }
    RowLayout {
        Label { text: qsTr("料盘2起点") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: tray_start_point2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point2.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: tray_start_point2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                tray_start_point2.setY(text)
            }
        }
        Button {
            text: title_move_to
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_START_POS)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                tray_start_point2.setX(x);
                tray_start_point2.setY(y);
            }
        }
    }
    SensorCarrierInfoAndPositionTestOperation {}
}
