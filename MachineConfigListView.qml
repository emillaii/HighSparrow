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
                case 4 : return "ImageGrabberView.qml"
                case 5 : return "CalibrationView.qml"
            }
        }
    }
}
