import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import TrayLoaderModuleLib 1.1
ColumnLayout{
    RowLayout{ Label { text: qsTr("点胶操作") } }
    RowLayout{
        Button{
            text: qsTr("AA1点胶")
        }
        Button{
            text: qsTr("AA1去UV")
        }
        Button{
            text: qsTr("SUT1 Load POS")
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
        Button{
            text: qsTr("SUT1 Clamp")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
    RowLayout{
        Button{
            text: qsTr("AA2点胶")
        }
        Button{
            text: qsTr("AA2去UV")
        }
        Button{
            text: qsTr("SUT2 Load POS")
        }
        Button{
            text: qsTr("SUT2 VAC")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
        Button{
            text: qsTr("SUT2 Clamp")
        }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }

    RowLayout{ Label { text: qsTr("AA1点胶调试") } }
    RowLayout{
        Button{
            text: qsTr("画胶PR位置")
        }
        Button{
            text: qsTr("点胶高度")
        }
        Label{
            text: qsTr("SUT Z")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text: qsTr("测高")
        }
        Button{
            text: qsTr("针头抬高")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("Dispensing Offset")
        }
        Label{
            text: qsTr("SUT X")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Label{
            text: qsTr("Y")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text: qsTr("视觉")
        }
        Button{
            text: qsTr("喷胶")
        }
        Button{
            text: title_read_encoder
        }
    }

    RowLayout{
        Button{
            text: qsTr("出胶延时")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text: qsTr("收胶延时")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text: qsTr("Dispensing Path")
        }
        Button{
            text: qsTr("Dispensing Line Speed")
        }
    }


    RowLayout{ Label { text: qsTr("AA2点胶调试") } }
    RowLayout{
        Button{
            text: qsTr("画胶PR位置")
        }
        Button{
            text: qsTr("点胶高度")
        }
        Label{
            text: qsTr("SUT Z")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text: qsTr("测高")
        }
        Button{
            text: qsTr("针头抬高")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
    }

    RowLayout{
        Button{
            text: qsTr("Dispensing Offset")
        }
        Label{
            text: qsTr("SUT X")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Label{
            text: qsTr("Y")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text: qsTr("视觉")
        }
        Button{
            text: qsTr("喷胶")
        }
        Button{
            text: title_read_encoder
        }
    }

    RowLayout{
        Button{
            text: qsTr("出胶延时")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text: qsTr("收胶延时")
        }
        TextField{
            text: ""
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
        }
        Button{
            text: qsTr("Dispensing Path")
        }
        Button{
            text: qsTr("Dispensing Line Speed")
        }
    }
}
