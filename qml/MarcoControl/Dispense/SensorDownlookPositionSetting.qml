import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import SutModuleLib 1.1
Column {
    RowLayout {
        Label { text: qsTr("Dispensing Glue PR Position") }
    }
    RowLayout {
        Label {
            text: qsTr("SUT 1 PR Position")
        }
        Label{
           text:qsTr("X")
        }
        TextField{
            text: tcpSUTDownlookPosition.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcpSUTDownlookPosition.setX(text)
           }
        }
        Label{
           text:qsTr("Y")
        }
        TextField{
           text: tcpSUTDownlookPosition.Y
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcpSUTDownlookPosition.setY(Z)
           }
        }
        Label{
           text:qsTr("Z")
        }
        TextField{
            text: tcpSUTDownlookPosition.Z
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
               tcpSUTDownlookPosition.setZ(text)
           }
        }
        Button{
           text:title_read_encoder
           onClicked: {
               var x = baseModuleManager.getMotorFeedbackPos(tcpSUTParams.motorXName)
               var y = baseModuleManager.getMotorFeedbackPos(tcpSUTParams.motorYName)
               var z = baseModuleManager.getMotorFeedbackPos(tcpSUTParams.motorZName)
               tcpSUTDownlookPosition.setX(x)
               tcpSUTDownlookPosition.setY(y)
               tcpSUTDownlookPosition.setZ(z)
           }
        }

        Button{
           text:title_move_to
           onClicked: {
               logicManager.performHandling(tcpSUTParams.moduleName, SutModule.DOWNLOOK_PR_POS)
           }
        }
    }
    RowLayout {
        Label {
            text: qsTr("SUT 2 PR Position")
        }
        Label{
           text:qsTr("X")
        }
        TextField{
           text: sutDownlookPosition.X
           horizontalAlignment: TextInput.AlignHCenter
           validator: DoubleValidator{
               decimals: 6
               notation: DoubleValidator.StandardNotation
           }
           onEditingFinished: {
           }
        }
        Label{
           text:qsTr("Y")
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
           text:qsTr("Z")
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
        Button{
           text:title_read_encoder
           onClicked: {
           }
        }

        Button{
           text:title_move_to
           onClicked: {
           }
        }
    }
}
