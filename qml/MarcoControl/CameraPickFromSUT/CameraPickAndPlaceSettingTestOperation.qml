import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../BaseUIModule"
Column {
    Label { text: qsTr("测试") }
    RowLayout {
        Button {
            text: qsTr("SUT1 Load Position")
        }
        Button {
            text: qsTr("CPA Pick And Place SUT1 PR Position")
        }
        Button {
            text: qsTr("SUT1 Place Camera")
        }
        Button {
            text: qsTr("SUT1 Pick Camera")
        }
    }
    RowLayout {
        Button {
            text: qsTr("SUT2 Load Position")
        }
        Button {
            text: qsTr("CPA Pick And Place SUT2 PR Position")
        }
        Button {
            text: qsTr("SUT2 Place Camera")
        }
        Button {
            text: qsTr("SUT2 Pick Camera")
        }
    }

    BaseSut1UI {}
    BaseSut2UI {}
}
