import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

Column{
    RowLayout {
        Label {
            text: qsTr("进盘弹夹层数")
        }
        TextField{
            id:col_in
            text:sensor_entrance_clip_parameter.currentIndex+1
            validator: IntValidator{
                bottom: 1
            }
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                var idx = text-1
                sensor_entrance_clip_parameter.setCurrentIndex(idx)
            }
        }
        Button{
            text:title_move_to
            onClicked: {
                logicManager.sensorTrayLoaderModuleSTIEMovetoColumnIndex(col_in.text-1)
            }
        }
        Button{
            text:qsTr("弹出")
            onClicked: {
                logicManager.sensorTrayLoaderModuleMovetoPushoutPosition()
            }
        }
        Button {
            text: qsTr("Boat1 取盘")
        }
        Button {
            text: qsTr("Boat2 取盘")
        }
    }
    RowLayout {
        Label {
            text: qsTr("出盘弹夹层数")
        }
        TextField{
            id:col_out
            text:sensor_exit_clip_parameter.currentIndex+1
            validator: IntValidator{
                bottom: 1
            }
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                var idx = text-1
                sensor_exit_clip_parameter.setCurrentIndex(idx)
            }
        }
        Button{
            text:qsTr("移动")
            onClicked: {
                logicManager.sensorTrayLoaderModuleSTOEMovetoColumnIndex(col_out.text-1)
            }
        }
        Button {
            text: qsTr("推盘")
            onClicked:  {
            }
        }
    }

}
