import QtQuick 2.9
import QtQuick.Controls 2.2
Button {
    id: button
    checkable: true
    font.pixelSize: 8
    leftPadding: 4
    rightPadding: 4
    topPadding: 12
    bottomPadding: 12
    implicitWidth: 60
    implicitHeight: 90

    icon.name: "placeholder"
    icon.width: 44
    icon.height: 44
    display: Button.TextUnderIcon

}
