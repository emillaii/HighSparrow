import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1
import LutModuleLib 1.1
import AACoreNew 1.1

RowLayout {
    Label { text: qsTr("AA 2") }
    Button {
        text: qsTr("Pick Pocket1 Position")
        onClicked: {
            logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS_POS)
        }
    }
    Button {
        text: qsTr("Pick Pocket2 Position")
        onClicked: {
            logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UNPICK_LENS_POS)
        }
    }
    Button {
        text: qsTr("Pick Lens Pocket 1")
        onClicked: {
            logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS)
        }
    }
    Button {
        text: qsTr("Place Lens Pocket 1")
        onClicked: {
            logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UNPICK_LENS)
        }
    }
    Button {
        text: qsTr("Pick Lens Pocket 2")
        onClicked: {
            logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS)
        }
    }
    Button {
        text: qsTr("Place Lens Pocket 2")
        onClicked: {
            logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS)
        }
    }
}
