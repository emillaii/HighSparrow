import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick 2.11
import QtQuick.Window 2.0

Window {
    property int count: 0
    property bool lightState: true
    id: alarmWindow
    color: "black"
    title: "Alarm Dialog"
    width: 900
    height: 300

    flags:  Qt.Window | Qt.WindowSystemMenuHint
            | Qt.WindowTitleHint
//            | Qt.WindowMinimizeButtonHint
//            | Qt.WindowMaximizeButtonHint

//    modality: Qt.NonModal // no need for this as it is the default value
    ListModel {
        id: myModel
    }
    ListView {
        anchors.fill: parent
        anchors.margins: 20
        clip: true
        model: myModel
        delegate: ColumnLayout {
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                Layout.fillHeight: true
                RowLayout{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Rectangle {
                        width:  160
                        height: 40
                        color: alarm_color
                        Label {
                            width:  160
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: module_name
                        }
                    }
                    Label {
                        verticalAlignment: Text.AlignVCenter
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: message_content
                    }
                    RowLayout{
                        Button {
                            id:button1
                            text: choose_tip1
                            visible: feedback1
                            onClicked: {
                                alarmShower.sendOperation(module_name,alarm_id,text)
                                myModel.remove(index)
                                if(myModel.count<=0)
                                {
                                    alarmWindow.visible = false
                                    alarmShower.closeLight()
                                }
                            }
                        }
                        Button {
                            id:button2
                            text: choose_tip2
                            visible: feedback2
                            onClicked: {
                                alarmShower.sendOperation(module_name,alarm_id,text)
                                myModel.remove(index)
                                if(myModel.count<=0)
                                {
                                    alarmWindow.visible = false
                                    alarmShower.closeLight()
                                }
                            }
                        }
                        Button {
                            id:button3
                            text: choose_tip3
                            visible: feedback3
                            onClicked: {
                                alarmShower.sendOperation(module_name,alarm_id,text)
                                myModel.remove(index)
                                if(myModel.count<=0)
                                {
                                    alarmWindow.visible = false
                                    alarmShower.closeLight()
                                }
                            }
                        }
                        Button {
                            id:button4
                            text: choose_tip4
                            visible: feedback4
                            onClicked: {
                                alarmShower.sendOperation(module_name,alarm_id,text)
                                myModel.remove(index)
                                if(myModel.count<=0)
                                {
                                    alarmWindow.visible = false
                                    alarmShower.closeLight()
                                }
                            }
                        }

                    }
                }
            }

        Connections{
            target: alarmShower
            onShowAlarm: {
                myModel.append({"alarm_id":alarmShower.getAlarmId(),
                                   "alarm_color":alarmShower.getAlarmColor(),
                                   "module_name":alarmShower.getModuleName(),
                                   "message_content":alarmShower.getMessageContent(),
                                   "choose_tip1":alarmShower.getChooseTip1(),
                                   "feedback1":alarmShower.getFeedBack1(),
                                   "choose_tip2":alarmShower.getChooseTip2(),
                                   "feedback2":alarmShower.getFeedBack2(),
                                   "choose_tip3":alarmShower.getChooseTip3(),
                                   "feedback3":alarmShower.getFeedBack3(),
                                   "choose_tip4":alarmShower.getChooseTip4(),
                                   "feedback4":alarmShower.getFeedBack4(),
                               })
                alarmShower.finihShow()
                alarmWindow.visible = true
            }
        }
    }
}

