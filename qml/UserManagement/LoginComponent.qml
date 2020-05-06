import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

ColumnLayout{
    function clearText()
    {
        txtUserName.clear()
        txtPassword.clear()
    }

    ChangePassword{
        id: popupChangePsw
    }

    signal closePopup()

    property bool cancelButtonVisible: true
    property bool changePswButtonVisible: true

    GridLayout{
        rows: 2
        columns: 2
        Label{
            text: qsTr("UserName:")
            horizontalAlignment: Text.AlignRight
        }
        TextField{
            id: txtUserName
            Layout.fillWidth: true
            selectByMouse: true
        }
        Label{
            text: qsTr("Password:")
            horizontalAlignment: Text.AlignRight
        }
        TextField{
            id: txtPassword
            echoMode: TextInput.Password
            Layout.fillWidth: true
            selectByMouse: true
        }
    }

    RowLayout{
        Button{
            text: qsTr("Login")
            onClicked: {
                if(userManagement.login(txtUserName.text, txtPassword.text))
                {
                    closePopup()
                }
            }
        }
        Button{
            visible: cancelButtonVisible
            text: qsTr("Cancel")
            onClicked: {
                closePopup()
            }
        }
        Button{
            visible: changePswButtonVisible
            text: qsTr("ChangePassword")
            onClicked: {
                popupChangePsw.clearText()
                popupChangePsw.open()
            }
        }
    }
}
