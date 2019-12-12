import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Popup {
    id: popupLogin
    x: 100
    y: 100
    width: 300
    height: 150

    closePolicy: Popup.NoAutoClose

    function clearText()
    {
        loginComponent.clearText()
    }

    contentItem: LoginComponent{
        id: loginComponent
        anchors.fill: parent
    }

    Connections{
        target: loginComponent
        onClosePopup: {
            popupLogin.close()
        }
    }
}


