import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

ComboBox {
    property var dataModel: null
    property var identity: null

    QtObject{
        id: self
        property var options: []
        property bool isInit: false
    }

    function init(dataModelIsConfigObj=true)
    {
        self.options = dataModel.getOptions(identity)
        model = self.options

        if(dataModelIsConfigObj){
            dataModel.connectNotifySignal(identity, this, "updateSelf")
        }

        updateSelf()
        self.isInit = true
    }

    function updateSelf()
    {
        var propertyValue = dataModel.getConfig(identity)
        for(var i = 0; i < self.options.length; i++){
            if(propertyValue === self.options[i]){
                currentIndex = i;
                return;
            }
        }
        currentIndex = -1;  //did not find correct option
    }

    onCurrentIndexChanged: {
        if(!self.isInit){
            return
        }
        if(!dataModel.setConfig(identity, self.options[currentIndex])){
            updateSelf()
        }
    }
}
