import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

GroupBox{

    ColumnLayout{
        Label{
            text: qsTr("点亮DOE")
        }
        RowLayout{
            Label{
                text:qsTr("DOE次数")
            }
            TextField{
                id: txtDoeTimes;
                text:"10"
                horizontalAlignment: TextInput.AlignHCenter
                validator: IntValidator{
                    bottom: 0
                }
            }
            Label{
                text:qsTr("Sensor数量")
            }
            TextField{
                id: txtSensorCount
                text:"10"
                horizontalAlignment: TextInput.AlignHCenter
                validator: IntValidator{
                    bottom:0
                }
            }
        }
        RowLayout{
            Label{
                text:qsTr("Pass:")
            }
            TextField {
                id: txtNPass
                text: qsTr("0")
                readOnly: true
                horizontalAlignment: TextInput.AlignHCenter
            }
            Label{
                text:qsTr("Fail:")
            }
            TextField {
                id: txtNFail
                text: qsTr("0")
                readOnly: true
                horizontalAlignment: TextInput.AlignHCenter
            }
        }
        RowLayout{
            Button{
                text: qsTr("Start")
                onClicked: {
                    startCameraDoe.startDOE(txtSensorCount.text, txtDoeTimes.text)
                }
            }
            Button{
                text: qsTr("Stop")
                onClicked: {
                    startCameraDoe.stopDOE()
                }
            }
        }

        Connections{
            target: startCameraDoe
            onPassFailCountChanged: {
                txtNFail.text = nFail
                txtNPass.text = nPass
            }
        }
    }
}

