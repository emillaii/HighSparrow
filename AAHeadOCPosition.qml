import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11


ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("AA_Z")
        }
        TextField {
            text: aaHeadParams.OCPositionZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 3
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                aaHeadParams.setOCPositionZ(text)
            }
        }
    }

    RowLayout {
        Button {
            text: title_move_to
            width: 40
            height: 40
            onClicked: {
                logicManager.aaMoveToOCPos()
            }
        }
        Button {
            text: title_read_encoder
            width: 20
            height: 40
            onClicked: {
                var z = baseModuleManager.getMotorFeedbackPos(m_AA1_Z)
                aaHeadParams.setOCPositionZ(z)
            }
        }
    }
}
