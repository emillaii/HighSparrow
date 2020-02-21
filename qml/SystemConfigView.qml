import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2

ItemDelegate {
    width: parent.width
    contentItem: ColumnLayout {
        spacing: 0

        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            GroupBox{
                title:qsTr("配置参数")
                ColumnLayout {
                    RowLayout {
                        Label {
                            text: qsTr("物料型号")
                        }
                        TextField {
                            text: moduleManagerParam.materialType
                            onEditingFinished: {
                                moduleManagerParam.setMaterialType(text)
                            }
                        }
                        Label {
                            text: qsTr("数据服务器地址")
                        }
                        TextField {
                            text: ""
                            onEditingFinished: {

                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("Lot Number:")
                        }
                        TextField {
                            text: aaCoreParams.lotNumber
                            onEditingFinished: {
                                aaCoreParams.setLotNumber(text)
                            }
                        }
                    }
                }
            }
        }
    }
}

