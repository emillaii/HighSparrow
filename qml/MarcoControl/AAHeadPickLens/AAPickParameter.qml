import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.2
import LogicManagerLib 1.1
import SomeLib 1.1

RowLayout{
    Label {
        text: qsTr("AA 1 抓取速度")
    }
    TextField {
        text: tcpLUTParams.pickSpeed
        horizontalAlignment: TextInput.AlignHCenter
        validator: DoubleValidator {
            decimals: 6
            notation: DoubleValidator.StandardNotation
        }
        onEditingFinished: {
            tcpLUTParams.setPickSpeed(text)
        }
    }
    Label {
        text: qsTr("AA 1 压力")
    }
    TextField {
        text: tcpLUTParams.pickForce
        horizontalAlignment: TextInput.AlignHCenter
        validator: DoubleValidator {
            decimals: 6
            notation: DoubleValidator.StandardNotation
        }
        onEditingFinished: {
            tcpLUTParams.setPickForce(text)
        }
    }
    Label {
        text: qsTr("AA 1 关爪延时")
    }
    TextField {
        text: tcpLUTParams.gripperDelay
        horizontalAlignment: TextInput.AlignHCenter
        validator: DoubleValidator {
            decimals: 6
            notation: DoubleValidator.StandardNotation
        }
        onEditingFinished: {
            tcpLUTParams.setGripperDelay(text)
        }
    }
    Label {
        text: qsTr("AA 2 抓取速度")
    }
    TextField {
        text: lutParams.pickSpeed
        horizontalAlignment: TextInput.AlignHCenter
        validator: DoubleValidator {
            decimals: 6
            notation: DoubleValidator.StandardNotation
        }
        onEditingFinished: {
            lutParams.setPickSpeed(text)
        }
    }
    Label {
        text: qsTr("AA 2 压力")
    }
    TextField {
        text: lutParams.pickForce
        horizontalAlignment: TextInput.AlignHCenter
        validator: DoubleValidator {
            decimals: 6
            notation: DoubleValidator.StandardNotation
        }
        onEditingFinished: {
            lutParams.setPickForce(text)
        }
    }
    Label {
        text: qsTr("AA 2 关爪延时")
    }
    TextField {
        text: lutParams.gripperDelay
        horizontalAlignment: TextInput.AlignHCenter
        validator: DoubleValidator {
            decimals: 6
            notation: DoubleValidator.StandardNotation
        }
        onEditingFinished: {
            lutParams.setGripperDelay(text)
        }
    }
}

