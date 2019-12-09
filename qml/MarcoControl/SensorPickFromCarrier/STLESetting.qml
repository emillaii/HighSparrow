import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
   RowLayout {
       Label { text: qsTr("STLE 起始位置") }
       TextField{
           text:sensor_entrance_clip_parameter.startPosition
           horizontalAlignment:TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensor_entrance_clip_parameter.setStartPosition(text)
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTIEName)
               sensor_entrance_clip_parameter.setStartPosition(x)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               logicManager.sensorTrayLoaderModuleSTIEMovetoColumnIndex(0)
           }
       }
       Label { text: qsTr("CTLE 起始位置") }
       TextField{
           text:sensor_exit_clip_parameter.startPosition
           horizontalAlignment:TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensor_exit_clip_parameter.setStartPosition(text)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               logicManager.sensorTrayLoaderModuleSTOEMovetoColumnIndex(0)
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTOEName)
               sensor_exit_clip_parameter.setStartPosition(x)
           }
       }
   }
   RowLayout {
       Label { text: qsTr("STLE 结束位置") }
       TextField{
           text:sensor_entrance_clip_parameter.endPosition
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensor_entrance_clip_parameter.setEndPosition(text)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               logicManager.sensorTrayLoaderModuleSTIEMovetoColumnIndex(sensor_clip_standard_parameter.count-1);
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTIEName)
               sensor_entrance_clip_parameter.setEndPosition(x)
           }
       }
       Label { text: qsTr("CTLE 结束位置") }
       TextField{
           text:sensor_exit_clip_parameter.endPosition
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               sensor_exit_clip_parameter.setEndPosition(text)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               logicManager.sensorTrayLoaderModuleSTOEMovetoColumnIndex(sensor_clip_standard_parameter.count-1)
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTOEName)
               sensor_exit_clip_parameter.setEndPosition(x)
           }
       }
   }
}
