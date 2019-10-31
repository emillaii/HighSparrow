import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

ColumnLayout{
    RowLayout { Label { text: qsTr("SUT Load Position") }}
    RowLayout {
        Label { text: qsTr("SUT1 Load Position") }
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
    }
    RowLayout {
        Label {
            text: { text: qsTr("SUT 2 Load Position") }
        }
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
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2)
            }
        }
    }
}
