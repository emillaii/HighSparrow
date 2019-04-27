import QtQuick 2.9
import QtQuick.Controls 2.2

ListModel {
    id: machineSettingModel
    ListElement {
        label: "AA 头机器配置"
        index: 1
    }
    ListElement {
        label: "SUT 配置"
        index: 2
    }
    ListElement {
        label: "LUT 配置"
        index: 3
    }
    ListElement {
        label: "Dispense 配置"
        index: 4
    }
    ListElement {
        label: "Image Grabber 配置"
        index: 5
    }
    ListElement {
        label: "Calibration 配置"
        index: 6
    }
    ListElement {
        label: "AA Core 配置"
        index: 7
    }
    ListElement {
        label: "System 配置"
        index: 8
    }
    ListElement{
        label:"LensPickArm 配置"
        index: 9
    }
    ListElement{
        label:"TrayLoader 配置"
        index:10
    }

}

