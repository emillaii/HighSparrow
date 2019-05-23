import QtQuick 2.0

ListView {
    id: machineConfigListView
    model: MachineSettingModel {
    }
    delegate:  Component {
        Loader {
            source: switch(index) {
                case 1 : return "AAHeadModuleView.qml"
                case 2 :
                    return "SUTModuleView.qml"
                case 3 :
                    if (baseModuleManager.getServerMode() == 0)
                        return "LUTModuleView.qml"
                    return "";
                case 4 : return "DispenseModuleView.qml"
                case 5 : return "ImageGrabberView.qml"
                case 6 : return "CalibrationView.qml"
                case 7 : return "AACoreView.qml"
                case 8 : return "SystemConfigView.qml"
                case 9 :
                    if (baseModuleManager.getServerMode() == 0)
                        return "LensPickArmModuleView.qml"
                    else
                        return "SensorLoaderModuleView.qml"
                case 10:
                    if (baseModuleManager.getServerMode() == 0)
                        return "TrayLoaderModuleView.qml"
                    else
                        return "SensorTrayLoaderViewModule.qml"
                case 11: return "ThreadManagerView.qml"
                case 12:return "CommonControl.qml"
                case 13:return "CheckProcess.qml"
            }
        }
    }
}
