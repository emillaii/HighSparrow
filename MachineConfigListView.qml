import QtQuick 2.0

ListView {
    id: machineConfigListView
    model: MachineSettingModel {
    }
    delegate:  Component {
        Loader {
            source: switch(index) {
                case 1 : return "AAHeadModuleView.qml"
                case 2 : return "SUTModuleView.qml"
                case 3 : return "LUTModuleView.qml"
                case 4 : return "DispenseModuleView.qml"
                case 5 : return "ImageGrabberView.qml"
                case 6 : return "CalibrationView.qml"
                case 7 : return "AACoreView.qml"
                case 8 : return "SystemConfigView.qml"
            }
        }
    }
}
