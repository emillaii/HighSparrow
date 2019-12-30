import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0


TextField {
    property var dataModel: null
    property var identity: null

    function init(dataModelIsConfigObj=true)
    {
        if(dataModelIsConfigObj){
            dataModel.connectNotifySignal(identity, this, "updateSelf")
        }
        if(dataModel.isReadOnly(identity)){
            enabled = false;
        }
        var propertyType = dataModel.getConfigType(identity)
        if(propertyType === ConfigElementType.Int) {
            validator = intValidator
        }else if(propertyType === ConfigElementType.Double){
            validator = doubleValidator
        }
        updateSelf()
    }

    function updateSelf()
    {
        text = dataModel.getConfig(identity)
    }

    onEditingFinished: {
        if(!dataModel.setConfig(identity, text)){
            updateSelf()
        }
    }

    IntValidator{
        id: intValidator
    }
    DoubleValidator{
        id: doubleValidator
    }
}
