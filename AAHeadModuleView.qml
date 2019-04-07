import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("")
        }
    }
    GroupBox{
        title: qsTr("OC Position")
        OCPositionView {}
    }
    GroupBox{
        title: qsTr("Pick Lens Position")
        PickLensPositionView {}
    }

    GroupBox{
        title: qsTr("Uplook PR Config")
        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("Load PR Name")
                }
                TextField {
                    id: uplookCameraPR
                    text: "PR"
                    horizontalAlignment: TextInput.AlignHCenter
                    onTextChanged: {
                        //aaHeadParams.setVisionUplookPR(text)
                    }
                }
                FileDialog {
                    id: loadfileDialog
                    title: "选择加载PR文件"
                    selectExisting: true
                    selectFolder: false
                    selectMultiple: false

                    nameFilters: ["avdata文件 (*.avdata)"]
                    onAccepted: {
                        uplookCameraPR.text = loadfileDialog.fileUrl
                        //aaHeadParams.setVisionUplookPR(uplookCameraPR.text)
                    }
                }

                Button {
                    text: qsTr("Load PR Result")
                    width: 20
                    height: 40
                    onClicked: {
                        loadfileDialog.open()
                    }
                }
            }
        }
    }
}
