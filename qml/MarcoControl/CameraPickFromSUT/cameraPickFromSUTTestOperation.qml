import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import LogicManagerLib 1.1
import SomeLib 1.1
import AACoreNew 1.1
import SutModuleLib 1.1

Column {
    RowLayout {
        Button {
            text: qsTr("SUT 1 Load Position")
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName,SutModule.LOAD_POS)
            }
        }

        Button {
            text: qsTr("CPA Pick And Place SUT 1 PR Position")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1)
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("SUT 2 Load Position")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName,SutModule.LOAD_POS)
            }
        }

        Button {
            text: qsTr("CPA Pick And Place SUT 2 PR Position")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2)
            }
        }
    }
}
