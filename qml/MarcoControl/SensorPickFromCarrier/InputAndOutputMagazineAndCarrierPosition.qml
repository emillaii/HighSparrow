import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import SomeLib 1.1
import LogicManagerLib 1.1

Column {
   RowLayout {
       Label { text: qsTr("弹夹层数") }
       TextField {
           text: sensor_clip_standard_parameter.count
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tray_clipin_parameter.setColumnCount(text)
           }
       }
       Label { text: qsTr("弹夹层距") }
       TextField {
           text: sensor_clip_standard_parameter.delta
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator {
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               var y1 = sensor_entrance_clip_parameter.startPosition;
               var y2 = sensor_entrance_clip_parameter.startPosition;
               var delta = (y2-y1)/(sensor_clip_standard_parameter.count-1);
               tray_clipin_parameter.setColumnDelta(delta);
           }
       }
   }

   STLESetting {}
   STLSetting {}
   STKSetting {}
   Label { text:qsTr("测试") }
   SensorCarrierInputAndOutputTestOperation {}

   RowLayout {
       Button{
           text: qsTr("STLE推杆")
           onClicked:
           {
               logicManager.performHandling(sensor_tray_loader_module_parameter.moduleName, SensorTrayLoaderModule.STPO_PUSH)
           }
       }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: {
                   if(sensorTrayLoaderModuleState.stpoPushState)
                       return "lightGreen"
                   else
                       return "red"
               }
           }
       }
       Button{
           text: qsTr("Boat Clamp")
           onClicked: {
               baseModuleManager.toogleIoState(sensor_tray_loader_module_parameter.cylinderGripperIoName)
           }
       }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: {
                   if(sensorTrayLoaderModuleState.cylinderGripperState)
                       return "lightGreen"
                   else
                       return "red"
               }
           }
       }
       Button{
           text: qsTr("Boat1 气缸")
           onClicked: {
               baseModuleManager.toogleIoState(sensor_tray_loader_module_parameter.cylinderHoldTrayIoName)
           }
       }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: {
                   if(sensorTrayLoaderModuleState.boat1CylinderState)
                       return "lightGreen"
                   else
                       return "red"
               }
           }
       }
       Button{
           text: qsTr("STK Clamp1")
           onClicked: {
               baseModuleManager.toogleIoState(sensor_tray_loader_module_parameter.cylinderSTK1IoName)
           }
       }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: {
                   if(sensorTrayLoaderModuleState.stk1ClampState)
                       return "lightGreen"
                   else
                       return "red"
               }
           }
       }
       Button{
           text: qsTr("Boat2 气缸")
           onClicked: {
               baseModuleManager.toogleIoState(sensor_tray_loader_module_parameter.cylinderVacancyTrayIoName)
           }
       }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: {
                   if(sensorTrayLoaderModuleState.boat2CylinderState)
                       return "lightGreen"
                   else
                       return "red"
               }
           }
       }
       Button{
           text: qsTr("STK Clamp2")
           onClicked: {
               baseModuleManager.toogleIoState(sensor_tray_loader_module_parameter.cylinderSTK2IoName)
           }
       }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: {
                   if(sensorTrayLoaderModuleState.stk2ClampState)
                       return "lightGreen"
                   else
                       return "red"
               }
           }
       }
   }
}
