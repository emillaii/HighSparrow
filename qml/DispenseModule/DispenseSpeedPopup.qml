import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11

Popup {
//    RowLayout {
//        Label {
//            text: qsTr("画胶速度参数")
//        }
        ScrollView {
            implicitWidth:  200
            implicitHeight: dispenserParams.lineSpeeds.length*45
            clip: true
            ListView {
                model: dispenserParams.lineSpeeds
                delegate: RowLayout {
                    Label { text: index + qsTr(":")}
                    TextField {
                        text: modelData
                        onEditingFinished: {
                            dispenserParams.setLineSpeed(index, text)
                        }
                    }
                }
            }
        }
//    }
}
