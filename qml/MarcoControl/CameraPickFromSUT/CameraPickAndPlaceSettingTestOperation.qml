import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../BaseUIModule"
import LogicManagerLib 1.1
import SomeLib 1.1
import AACoreNew 1.1
import SutModuleLib 1.1
Column {
    Label { text: qsTr("测试") }
    RowLayout {
        Button {
            text: qsTr("SUT1 Load Position")
            onClicked: {
                logicManager.performHandling(tcpSUTParams.moduleName,SutModule.LOAD_POS)
            }
        }
        Button {
            text: qsTr("CPA Pick And Place SUT1 PR Position")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1)
            }
        }
        Button {
            text: qsTr("SUT1 Place Camera")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.TO_PLACE_SENSOR_POS1
                                             +SensorLoaderModule.PLACE_SENSOR_TO_SUT1)
            }
        }
        Button {
            text: qsTr("SUT1 Pick Camera")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS1
                                             +SensorLoaderModule.TO_PICK_PRODUCT_POS1
                                             +SensorLoaderModule.PICK_PRODUCT_FROM_SUT1)
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("SUT2 Load Position")
            onClicked: {
                logicManager.performHandling(sutParams.moduleName,SutModule.LOAD_POS)
            }
        }
        Button {
            text: qsTr("CPA Pick And Place SUT2 PR Position")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2)
            }
        }
        Button {
            text: qsTr("SUT2 Place Camera")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.TO_PLACE_SENSOR_POS2
                                             +SensorLoaderModule.PLACE_SENSOR_TO_SUT2)
            }
        }
        Button {
            text: qsTr("SUT2 Pick Camera")
            onClicked: {
                logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SUT_POS2
                                             +SensorLoaderModule.TO_PICK_PRODUCT_POS2
                                             +SensorLoaderModule.PICK_PRODUCT_FROM_SUT2)
            }
        }
    }

    BaseSut1UI {}
    BaseSut2UI {}
}
