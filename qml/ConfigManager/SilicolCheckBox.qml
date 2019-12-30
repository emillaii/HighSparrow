import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

CheckBox {
    property var dataModel: null
    property var identity: null

    function init(dataModelIsConfigObj=true)
    {
        if(dataModelIsConfigObj){
            dataModel.connectNotifySignal(identity, this, "updateSelf")
        }
        if(dataModel.isReadOnly(identity)){
            enabled = false
        }
        updateSelf()
    }

    function updateSelf()
    {
        checked = dataModel.getConfig(identity)
    }

    onClicked: {
        if(!dataModel.setConfig(identity, checked)){
            updateSelf()
        }
    }
}
