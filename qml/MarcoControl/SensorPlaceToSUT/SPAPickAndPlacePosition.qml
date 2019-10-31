import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout{
    RowLayout { Label { text: qsTr("SPA Pick And Place Position") }}
    RowLayout{
        Label{
            text:qsTr("吸头1吸料角度")
        }
        TextField{
            text:sensorLoaderParameter.picker1PickTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPicker1PickTheta(text)
            }
        }
        Label{
            text:qsTr("吸头1放料角度")
        }
        TextField{
            text:sensorLoaderParameter.picker1PlaceTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPicker1PlaceTheta(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("吸头2吸料角度")
        }
        TextField{
            text:sensorLoaderParameter.picker2PickTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPicker2PickTheta(text)
            }
        }
        Label{
            text:qsTr("吸头2放料角度")
        }
        TextField{
            text:sensorLoaderParameter.picker2PlaceTheta
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPicker2PlaceTheta(text)
            }
        }
    }
    RowLayout {
        Label { text: qsTr("SPA Pick & Place SUT1 Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: sut_pr_position1.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sut_pr_position1.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: sut_pr_position1.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sut_pr_position1.setY(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                sut_pr_position1.setX(x);
                sut_pr_position1.setY(y);
            }
        }
        Button {
            text: title_move_to
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1)
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
            }
        }
        Button {
            text: qsTr("Place Sensor视觉")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.TO_PLACE_SENSOR_POS1)
            }
        }
        Button{
            text:qsTr("放sensor")
            width: 40
            height: 40
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.TO_PLACE_SENSOR_POS1
                                             +SensorLoaderModule.PLACE_SENSOR_TO_SUT1)
            }
        }
        Button {
            text: qsTr("NG ensor视觉")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.SUT_SENSOR_PR
                                             +SensorLoaderModule.TO_PR_OFFSET)
            }
        }
        Button {
            text: qsTr("移动到取放位置")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.SUT_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_NG_SENSOR_POS1)
            }
        }
        Button {
            text: qsTr("取NG ensor")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.SUT_SENSOR_PR
                                             +SensorLoaderModule.TO_PICK_NG_SENSOR_POS1
                                             +SensorLoaderModule.PICK_NG_SENSOR_FROM_SUT1)
            }
        }
    }
    RowLayout {
        Label { text: qsTr("SPA Pick & Place SUT2 Position") }
        Label {
            text: qsTr("X")
        }
        TextField {
            text: sut_pr_position2.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sut_pr_position2.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: sut_pr_position2.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sut_pr_position2.setY(text)
            }
        }
        Button {
            text: title_read_encoder
            onClicked: {
                var x = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorXName)
                var y = baseModuleManager.getMotorFeedbackPos(sensorPickArmParams.motorYName)
                sut_pr_position2.setX(x);
                sut_pr_position2.setY(y);
            }
        }
        Button {
            text: title_move_to
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1)
            }
        }
        Button {
            text: qsTr("sensor视觉")
            onClicked: {
            }
        }
        Button {
            text: qsTr("移动到取放位置")
            onClicked: {
            }
        }
        Button {
            text: qsTr("测高")
            onClicked: {
            }
        }
    }
    RowLayout {
        Label{
            text:qsTr("SUT Escape")
        }
        Label{
            text:qsTr("X")
        }
        TextField{
            text: sensorLoaderParameter.escapeX
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setEscapeX(text)
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text: sensorLoaderParameter.escapeY
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setEscapeY(text)
            }
        }
        Label{
            text:qsTr("Z")
        }
        TextField{
            text: sensorLoaderParameter.escapeHeight
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setEscapeHeight(text)
            }
        }
    }
}
