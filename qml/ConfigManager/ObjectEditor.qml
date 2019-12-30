import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

BasicConfigEditor{
    function init(_isExpand=true)
    {
        isExpand = _isExpand
        configRepeter.model = configModel.getConfigNamesToShow()
    }

    ColumnLayout{
        visible: isExpand
        spacing: 5

        Repeater{
            id: configRepeter
            delegate: Frame{
                background: Rectangle{
                    radius: 5
                    border.width: 1
                }
                padding: 5

                Loader{
                    source: {
                        var elementType = configModel.getConfigType(modelData)
                        if (elementType === ConfigElementType.ConfigObj){
                            return "ObjectEditor.qml"
                        }else if(elementType === ConfigElementType.ConfigArr ||
                                 elementType === ConfigElementType.ConfigObjArr){
                            return "ArrayEditor.qml"
                        }else {
                            return "ConfigRow.qml"
                        }
                    }

                    onLoaded: {
                        var elementType = configModel.getConfigType(modelData)
                        if(elementType === ConfigElementType.ConfigObj ||
                                elementType === ConfigElementType.ConfigArr ||
                                elementType === ConfigElementType.ConfigObjArr)
                        {
                            item.configModel = configModel.getConfig(modelData)
                            item.sectionName = modelData
                            item.listMaxWidth = listMaxWidth
                            item.listMaxHeight = listMaxHeight
                            item.init(false)
                        }else{
                            item.configModel = configModel
                            item.configName = modelData
                            item.init(elementType)
                        }
                    }
                }
            }
        }
    }
}
