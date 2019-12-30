import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

BasicArrayRow{
    id: configNode

    property var childItem: null
    property alias connRowDataChanged: connRowDataChanged

    function init(elementType)
    {
        var component = null
        if(elementType === ConfigElementType.Bool){
            component = Qt.createComponent("SilicolCheckBox.qml")
        }else if(elementType === ConfigElementType.Int ||
                 elementType === ConfigElementType.Double ||
                 elementType === ConfigElementType.Other){
            if(configModel.isOptional()){
                component = Qt.createComponent("SilicolComboBox.qml")
            }else{
                component = Qt.createComponent("SilicolTextField.qml")
            }
        }
        if(component !== null){
            var obj = component.createObject(configNode)
            if(obj === null){
                console.error(component.errorString())
                return
            }
            obj.dataModel = configModel
            obj.identity = index
            obj.init(false)
            childItem = obj
        }else{
            console.log("Unknown type: " + elementType)
            return
        }

        connRowIndexChanged.target = rowModel
        connRowDataChanged.target = rowModel
    }

    Binding{
        target: childItem
        property: "identity"
        value: index
    }

    Connections{
        id: connRowDataChanged
        target: null
        onRowDataChanged: {
            childItem.updateSelf()
        }
    }
}
