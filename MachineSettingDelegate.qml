import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11

ItemDelegate {
    id: root
    width: parent.width
    checkable: true

    onClicked: {
        ListView.view.currentIndex = index
    }
    contentItem: ColumnLayout {
        spacing: 0
        RowLayout {
            ColumnLayout {
                RowLayout {
                    Label {
                        id: itemLabel
                        text: model.label
                        font.pixelSize: 20
                    }
                }
            }
            Item {
                Layout.fillWidth: true
            }
        }

        ColumnLayout {
            visible: {
                if (model.index === 1)
                    return root.checked
                else
                    return false
            }
            AAHeadModuleView {
            }
        }
        ColumnLayout {
            visible: {
                if (model.index === 2)
                    return root.checked
                else
                    return false
            }
            SUTModuleView {
            }
        }
        ColumnLayout {
            visible: {
                if (model.index === 3)
                    return root.checked
                else
                    return false
            }
            LUTModuleView {
            }
        }
        ColumnLayout {
            visible: {
                if (model.index === 4)
                    return root.checked
                else
                    return false
            }
            ImageGrabberView {
            }
        }
    }
}
