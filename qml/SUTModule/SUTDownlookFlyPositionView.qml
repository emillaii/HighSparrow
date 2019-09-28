import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

ColumnLayout {
    RowLayout {
        Label {
            text: qsTr("SUT Downlook Fly PR Y")
        }
        TextField {
            text: sutParams.downlookFlyPrPosition
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sutParams.setDownlookFlyPrPosition(text)
            }
        }
        Label {
            text: qsTr("SUT Downlook Fly Start Y")
        }
        TextField {
            text: sutParams.downlookFlyStartPosition
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sutParams.setDownlookFlyStartPosition(text)
            }
        }
    }

    RowLayout {
        Label {
            text: qsTr("SUT Downlook Fly End Position X")
        }
        TextField {
            text: sutDownlookFlyEndPosition.X
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sutDownlookFlyEndPosition.setX(text)
            }
        }
        Label {
            text: qsTr("Y")
        }
        TextField {
            text: sutDownlookFlyEndPosition.Y
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                 sutDownlookFlyEndPosition.setY(text)
            }
        }
    }

    RowLayout {
        Label {
            text: qsTr("OTF Velocity")
        }
        TextField {
            text: sutParams.downlookFlyVelocity
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sutParams.setDownlookFlyVelocity(text)
            }
        }

        Label {
            text: qsTr("Downlook Camera Exposure Time")
        }
        TextField {
            text: sutParams.downlookCameraExposureTime
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator {
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                sutParams.setDownlookCameraExposureTime(text)
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("OTF")
            width: 40
            height: 40
            onClicked: {
//                sutModule.performHandling(2)
                logicManager.performHandling(sutParams.moduleName,2)
            }
        }
        Button {
            text: qsTr("Set Exposure Time")
            width: 20
            height: 40
            onClicked: {
                downlookCamera.setCameraExposureTime(sutParams.downlookCameraExposureTime)
            }
        }
    }
}
