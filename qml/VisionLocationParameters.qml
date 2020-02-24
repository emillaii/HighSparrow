import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Popup {
    implicitWidth: 500
    implicitHeight: 800

    contentItem: Rectangle{
        id: rectangle
        color: "black"
        implicitWidth: 900
        implicitHeight: 900

        ListView{
            anchors.fill: parent
            model: vl_parameter_list

            delegate: GroupBox{
                title: locationName
                RowLayout{
                    Label{
                        text: qsTr("OffsetX:")
                    }
                    TextField {
                        text: offsetX
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: DoubleValidator {
                            decimals: 6
                            notation: DoubleValidator.StandardNotation
                        }
                        onEditingFinished: {
                            setOffsetX(text)
                        }
                    }
                    Label{
                        text: qsTr("Y:")
                    }
                    TextField {
                        text: offsetY
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: DoubleValidator {
                            decimals: 6
                            notation: DoubleValidator.StandardNotation
                        }
                        onEditingFinished: {
                            setOffsetY(text)
                        }
                    }
                    Label{
                        text: qsTr("Theta:")
                    }
                    TextField {
                        text: offsetTheta
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: DoubleValidator {
                            decimals: 6
                            notation: DoubleValidator.StandardNotation
                        }
                        onEditingFinished: {
                            setOffsetTheta(text)
                        }
                    }
                }
            }

            ScrollBar.vertical: ScrollBar{

            }
        }
    }
}
