import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

RowLayout{
    id: configRow
    property var configModel: null
    property string configName: "name"

    Label{
        text: configName + ":"
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
    }

    function init(elementType)
    {
        var component = null
        if(elementType === ConfigElementType.Bool){
            component = Qt.createComponent("SilicolCheckBox.qml")
        }else if(elementType === ConfigElementType.Int ||
                 elementType === ConfigElementType.Double ||
                 elementType === ConfigElementType.Other){
            if(configModel.isOptional(configName)){
                component = Qt.createComponent("SilicolComboBox.qml")
            }else{
                component = Qt.createComponent("SilicolTextField.qml")
            }
        }
        if(component !== null){
            var obj = component.createObject(configRow)
            if(obj === null){
                console.log("error")
                console.error(component.errorString())
                return
            }
            obj.dataModel = configModel
            obj.identity = configName
            obj.init()
        }else{
            console.log("Unknown type: " + elementType)
            return
        }
    } 
}
