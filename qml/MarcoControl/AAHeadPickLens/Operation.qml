import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import LogicManagerLib 1.1
import SomeLib 1.1
import LutModuleLib 1.1
import AACoreNew 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("AA1 Head操作") } }
    RowLayout{
        Button{
            text: qsTr("AA1 HEAD Pick Pocket1 POS")
            onClicked: {
                logicManager.performHandling(tcpAACoreParams.moduleName, AACoreNew.AA_HEAD_MOVE_TO_PICK_LENS)
            }
        }
        Button{
            text: qsTr("AA1 HEAD Pick Pocket1")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_PICK_LENS)
            }
        }
        Button{
            text: qsTr("AA1 HEAD Place Pocket1")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_UNPICK_LENS_FROM_POCKET1)
            }
        }
    }
    RowLayout{
        Button{
            text: qsTr("AA1 HEAD Pick Pocket2 POS")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.AA_HEAD_MOVE_TO_PICK_LENS)
            }
        }
        Button{
            text: qsTr("AA1 HEAD Pick Pocket2")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_PICK_LENS_FROM_POCKET2)
            }
        }
        Button{
            text: qsTr("AA1 HEAD Place Pocket2")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_UNPICK_LENS)
            }
        }
    }
    RowLayout{
        Button{
            text: qsTr("AA1 HEAD Up Look POS")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA1_UPLOOK_POS)
            }
        }
        Button{
            text: qsTr("AA1 Gripper Open")
            onClicked: {
                 baseModuleManager.toogleIoState(tcpAAHeadParams.gripperName, true)
            }
        }
        Button{
            text: qsTr("AA1 Gripper Close")
            onClicked: {
                 baseModuleManager.toogleIoState(tcpAAHeadParams.gripperName, false)
            }
        }
    }

    RowLayout{ Label { text: qsTr("AA2 Head操作") } }
    RowLayout{
        Button{
            text: qsTr("AA2 HEAD Pick Pocket1 POS")
            onClicked: {
                logicManager.performHandling(aaCoreParams.moduleName, AACoreNew.AA_HEAD_MOVE_TO_PICK_LENS)
            }
        }
        Button{
            text: qsTr("AA2 HEAD Pick Pocket1")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS)
            }
        }
        Button{
            text: qsTr("AA2 HEAD Place Pocket1")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UNPICK_LENS_FROM_POCKET1)
            }
        }
    }
    RowLayout{
        Button{
            text: qsTr("AA2 HEAD Pick Pocket2 POS")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_PICK_LENS_FROM_POCKET2)
            }
        }
        Button{
            text: qsTr("AA2 HEAD Pick Pocket2")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UNPICK_LENS)
            }
        }
        Button{
            text: qsTr("AA2 HEAD Place Pocket2")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UNPICK_LENS_FROM_POCKET1)
            }
        }
    }
    RowLayout{
        Button{
            text: qsTr("AA2 HEAD Up Look POS")
            onClicked: {
                logicManager.performHandling(tcpLUTParams.moduleName,LutModule.AA2_UPLOOK_POS)
            }
        }
        Button{
            text: qsTr("AA2 Gripper Open")
            onClicked: {
                 baseModuleManager.toogleIoState(aaHeadParams.gripperName, true)
            }
        }
        Button{
            text: qsTr("AA2 Gripper Close")
            onClicked: {
                 baseModuleManager.toogleIoState(aaHeadParams.gripperName, false)
            }
        }
    }
}
