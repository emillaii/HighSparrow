import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

Column {
   RowLayout {
       Label { text: qsTr("STL夹盘位置") }
       TextField{
           text:sensor_tray_loader_module_parameter.getTrayPosition
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               sensor_tray_loader_module_parameter.setGetTrayPosition(text)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               logicManager.sensorTrayLoaderModuleMovetoGetTrayPosition()
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorTrayName)
               sensor_tray_loader_module_parameter.setGetTrayPosition(x)
           }
       }
       Label { text: qsTr("STL夹盘结束位置") }
       TextField{
           text:sensor_tray_loader_module_parameter.finishKickTrayPosition
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               sensor_tray_loader_module_parameter.setFinishKickTrayPosition(text)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               logicManager.sensorTrayLoaderModuleMovetoFinishKickTrayPosition()
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTKName)
               sensor_tray_loader_module_parameter.setFinishKickTrayPosition(x)
           }
       }
   }
   RowLayout {
       Label { text: qsTr("STL Load 位置") }
       TextField{
           text:sensor_tray_loader_module_parameter.downTrayPosition
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               sensor_tray_loader_module_parameter.setDownTrayPosition(text)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               logicManager.sensorTrayLoaderModuleMovetoDownTrayPosition()
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorTrayName)
               sensor_tray_loader_module_parameter.setDownTrayPosition(x)
           }
       }
       Label { text: qsTr("STL 工作位置") }
       TextField{
           text:sensor_tray_loader_module_parameter.trayWorkPosition
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               sensor_tray_loader_module_parameter.setTrayWorkPosition(text)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               sensorTrayLoaderModule.movetoTrayWorkPosition()
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorTrayName)
               sensor_tray_loader_module_parameter.setTrayWorkPosition(x)
           }
       }
   }
   RowLayout {
       Label { text: qsTr("STL Unload 位置") }
       TextField{
           text:sensor_tray_loader_module_parameter.vacancyTrayPosition
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           horizontalAlignment: TextInput.AlignHCenter
           onEditingFinished: {
               sensor_tray_loader_module_parameter.setVacancyTrayPosition(text)
           }
       }
       Button{
           text:title_move_to
           onClicked: {
               logicManager.sensorTrayLoaderModuleMovetoVacancyTrayPosition()
           }
       }
       Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(sensor_tray_loader_module_parameter.motorSTKName)
               sensor_tray_loader_module_parameter.setVacancyTrayPosition(x)
           }
       }
   }
}
