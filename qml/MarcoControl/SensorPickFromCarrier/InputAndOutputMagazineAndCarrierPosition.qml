
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

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
       Button{ text: qsTr("STLE推杆") }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: "red"
           }
       }
       Button{ text: qsTr("Boat Clamp") }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: "red"
           }
       }
       Button{ text: qsTr("Boat1 气缸") }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: "red"
           }
       }
       Button{ text: qsTr("STK Clamp1") }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: "red"
           }
       }
       Button{ text: qsTr("Boat2 气缸") }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: "red"
           }
       }
       Button{ text: qsTr("STK Clamp2") }
       RoundButton{
           background: Rectangle {
               radius: 6
               color: "red"
           }
       }
   }
}
