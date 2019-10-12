import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

RowLayout{
    Button{
        text: qsTr("SUT1 Clamp")
    }
    RoundButton{
        background: Rectangle {
            radius: 6
            color: "red"
        }
    }

    Button{
        text: qsTr("SUT1 VAC")
    }
    RoundButton{
        background: Rectangle {
            radius: 6
            color: "red"
        }
    }
}
