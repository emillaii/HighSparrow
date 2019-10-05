import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import "./LensPickFromTray"
import "./AAHeadPickLens"
ScrollView {
    id: machineConfigListView
    width: 1200
    anchors.bottom: parent.bottom
    anchors.top: parent.top
    anchors.topMargin: 10
    anchors.bottomMargin: 10
    anchors.leftMargin: 10
    ColumnLayout{
        width: 1200
        Button {
            id: lensPickFromTrayViewSwitch;
            text:"Lens Pick From Tray";
            Layout.fillWidth: true
            onClicked: {
                lensPickFromTrayView.visible = !lensPickFromTrayView.visible
            }
        }
        LensPickFromTrayView {
            id: lensPickFromTrayView
            visible: false
        }
        Button {id: lensPlaceToLUTViewSwitch; text:"Lens Place To LUT";Layout.fillWidth: true}
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
        Button {id: sensorPickFromCarrierViewSwitch; text:"Sensor Pick From Carrier";Layout.fillWidth: true}
        Button {id: sensorPlaceToSUTViewSwitch; text:"Sensor Place To SUT";Layout.fillWidth: true}
        Button {id: sensorDispensingViewSwitch; text:"Sensor Dispensing";Layout.fillWidth: true}
        Button {id: activeAlignmentViewSwitch; text:"Active Alignment";Layout.fillWidth: true}
        Button {id: cameraPickFromSUTViewSwitch; text:"Camera Pick From SUT";Layout.fillWidth: true}
        Button {id: cameraPlaceToCarrierViewSwitch; text:"Camera Place To Carrier";Layout.fillWidth: true}
        Button {id: bufferAndRejectCarrierViewSwitch; text:"Buffer & Reject Carrier";Layout.fillWidth: true}
    }
}
