import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick 2.11
import QtQuick.Window 2.0
import LogicManagerLib 1.1
import SomeLib 1.1
RowLayout {
    Layout.fillWidth: false
    transformOrigin: Item.Left
    RoundButton {
        text: "清空"
        transformOrigin: Item.Center
        display: Button.TextBesideIcon
        icon.width: 30
        icon.height: 30
        icon.source: "../../icons/camera.png"
        icon.color: "lightGreen"
        onClicked: {
            logicManager.performHandling(tcp_lens_loader_parameter.moduleName,LensLoaderModule.CLEARANCE)
        }
    }
}
