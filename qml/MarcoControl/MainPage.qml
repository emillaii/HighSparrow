import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import "./LensPickFromTray"
import "./LensPlaceToLUT"
import "./AAHeadPickLens"
import "./SensorPickFromCarrier"
import "./SensorPlaceToSUT"
import "./Dispense"
import "./ActiveAlignment"
import "./CameraPickFromSUT"
import "./CameraPlaceToCarrier"
import "./BufferAndRejectCarrier"
import "./SystemConfiguration"

ScrollView {
    id: machineConfigListView
    width: 1600
    anchors.bottom: parent.bottom
    anchors.top: parent.top
    anchors.topMargin: 10
    anchors.bottomMargin: 10
    anchors.leftMargin: 10
    ColumnLayout{
        width: 1200
        Button {
            id: lensPickFromTrayViewSwitch
            text:"Lens Pick From Tray"
            Layout.fillWidth: true
            onClicked: {
                lensPickFromTrayView.visible = !lensPickFromTrayView.visible
            }
        }
        LensPickFromTrayView {
            id: lensPickFromTrayView
            visible: false
        }

        Button {
            id: lensPlaceToLUTViewSwitch
            text:"Lens Place To LUT"
            Layout.fillWidth: true
            onClicked: {
                lensPlaceToLUTView.visible = !lensPlaceToLUTView.visible
            }
        }
        LensPlaceToLUTView{
            id: lensPlaceToLUTView
            visible: false
        }

        Button {
            id: aaHeadPickLensViewSwitch
            text:"AA Head Pick Lens"
            Layout.fillWidth: true
            onClicked: {
                aaHeadPickLensView.visible = !aaHeadPickLensView.visible
            }
        }
        AAHeadPickLensView {
            id: aaHeadPickLensView
            visible: false
        }
        Button {
            id: sensorPickFromCarrierViewSwitch
            text:"Sensor Pick From Carrier"
            Layout.fillWidth: true
            onClicked: {
                sensorPickFromCarrierView.visible = !sensorPickFromCarrierView.visible
            }
        }
        SensorPickFromCarrierView {
            id: sensorPickFromCarrierView
            visible: false
        }

        Button {
            id: sensorPlaceToSUTViewSwitch
            text:"Sensor Place To SUT"
            Layout.fillWidth: true
            onClicked: {
                sensorPlaceToSUTView.visible = !sensorPlaceToSUTView.visible
            }
        }
        SensorPlaceToSUT {
            id: sensorPlaceToSUTView
            visible: false
        }

        Button {
            id: sensorDispensingViewSwitch;
            text:"Sensor Dispensing";
            Layout.fillWidth: true
            onClicked: {
                sensorDispensingView.visible = !sensorDispensingView.visible
            }
        }

        DispenseView {
            id: sensorDispensingView
            visible: false
        }

        Button {
            id: activeAlignmentViewSwitch
            text:"Active Alignment"
            Layout.fillWidth: true
            onClicked: {
                activeAlignmentView.visible = !activeAlignmentView.visible
            }
        }

        ActiveAlignmentView {
            id: activeAlignmentView
            visible: false
        }

        Button {
            id: cameraPickFromSUTViewSwitch
            text:"Camera Pick From SUT"
            Layout.fillWidth: true
            onClicked: {
                cameraPickFromSUTView.visible = !cameraPickFromSUTView.visible
            }
        }

        CameraPickFromSUTView {
            id: cameraPickFromSUTView
            visible: false
        }

        Button {
            id: cameraPlaceToCarrierViewSwitch
            text:"Camera Place To Carrier"
            Layout.fillWidth: true
            onClicked: {
                cameraPlaceToCarrierView.visible = !cameraPlaceToCarrierView.visible
            }
        }
        CameraPlaceToCarrierView{
            id: cameraPlaceToCarrierView
            visible: false
        }

        Button {
            id: bufferAndRejectCarrierViewSwitch
            text:"Buffer and Reject Carrier"
            Layout.fillWidth: true
            onClicked: {
                bufferAndRejectCarrierView.visible = !bufferAndRejectCarrierView.visible
            }
        }
        BufferAndRejectCarrierView{
            id: bufferAndRejectCarrierView
            visible: false
        }

        Button {
            id: systemConfigurationViewSwitch
            text:"System Configuration"
            Layout.fillWidth: true
            onClicked: {
                systemConfiguration.visible = !systemConfiguration.visible
            }
        }
        SystemConfiguration {
            id: systemConfiguration
            visible: false
        }
    }
}
