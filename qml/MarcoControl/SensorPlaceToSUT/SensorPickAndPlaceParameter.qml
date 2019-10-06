import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
    RowLayout {
        Label { text: qsTr("取料力度") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
    }
    RowLayout {
        Label { text: qsTr("SUT 1 放料区间") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePlaceToSutForceLimmit
            onCheckedChanged:
                sensorLoaderParameter.setDisablePlaceToSutForceLimmit(checked)
        }
        Label{
            text:qsTr("SUT 1 取料区间")
        }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePlaceToSutForceLimmit
            onCheckedChanged:
                sensorLoaderParameter.setDisablePlaceToSutForceLimmit(checked)
        }
    }
    RowLayout {
        Label { text: qsTr("SUT 2 放料区间") }
        TextField{
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        CheckBox{
            text: qsTr("不限力")
            checked: sensorLoaderParameter.disablePlaceToSutForceLimmit
            onCheckedChanged:
                sensorLoaderParameter.setDisablePlaceToSutForceLimmit(checked)
        }
        Label{
            text:qsTr("SUT 2 取料区间")
        }
        TextField{
            text:sensorLoaderParameter.pickFromSutMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sensorLoaderParameter.setPickFromSutMargin(text)
            }
        }
        CheckBox{
           text: qsTr("不限力")
           checked: sensorLoaderParameter.disablePickFromSutForceLimit
           onCheckedChanged:
               sensorLoaderParameter.setDisablePickFromSutForceLimit(checked)
       }
    }

}
