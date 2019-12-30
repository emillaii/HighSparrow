import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

BasicConfigEditor{
    QtObject{
        id: self
        property bool isObjectArray: false
        property int elementType: -1
        property var configNames: []
    }

    function init(_isExpand=true)
    {
        isExpand = _isExpand
        if(configModel.configType() === ConfigElementType.ConfigObjArr){
            self.isObjectArray = true
            self.configNames = configModel.getTitles()
            var configTitles = []
            configTitles.push("index")
            for(var i in self.configNames){
                configTitles.push(self.configNames[i])
            }
            titleRepeter.model = configTitles

        }else{
            self.isObjectArray = false
            self.elementType = configModel.getConfigType()
        }

        var dataCount = configModel.configCount()
        for(var i = 0; i < dataCount; i++){
            listModel.append({"rowIndex": i, "rowData": configModel.getConfig(i)})
        }

        connConfigInserted.target = configModel
        connConfigRemoved.target = configModel
        if(!self.isObjectArray){
            connConfigChanged.target = configModel
        }
    }

    ListModel{
        id: listModel
    }

    Connections{
        id: connConfigChanged
        target: null
        onConfigChanged:{
            listModel.setProperty(index, "rowData", value)
        }
    }
    Connections{
        id: connConfigInserted
        target: null
        onConfigInserted:{
            listModel.insert(index, {"rowIndex": index, "rowData": value})
            for(var i = index; i < listModel.count; i++){
                listModel.setProperty(i, "rowIndex", i)
            }
        }
    }
    Connections{
        id: connConfigRemoved
        target: null
        onConfigRemoved:{
            listModel.remove(index)
            for(var i = index; i < listModel.count; i++){
                listModel.setProperty(i, "rowIndex", i)
            }
            configNodes.resize()
        }
    }

    RowLayout{
        id: rowAddRemove
        visible: isExpand
        RoundButton{
            icon.source: "/icons/add.png"
            icon.color: "transparent"
            onClicked: {
                var listviewIndex = configNodes.currentIndex
                if(listviewIndex >= 0 && listviewIndex < configNodes.count){
                    configModel.add(listviewIndex + 1)
                }else{
                    configModel.add(configNodes.count)
                }
            }
        }
        RoundButton{
            icon.source: "/icons/remove.png"
            icon.color: "transparent"
            onClicked: {
                var listviewIndex = configNodes.currentIndex
                if(listviewIndex >= 0 && listviewIndex < configNodes.count){
                    configModel.remove(listviewIndex)
                }
            }
        }
    }

    Flickable{
        visible: self.isObjectArray && isExpand && listModel.count > 0
        implicitWidth: configNodes.implicitWidth
        height: 15
        clip: true
        contentX: hbar.position * configNodes.contentWidth

        Row{
            id: titleRow
            Repeater{
                id: titleRepeter
                delegate: Rectangle{
                    width: 30
                    height: 18
                    color: index % 2 == 0 ? "lightGreen" : "lightGray"
                    Text {
                        anchors.fill: parent
                        text: modelData
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            function resize(item)
            {
                var subItemsWidth = item.getItemsWidth()
                var itemsSpacing = item.getItemsSpacing()
                for(var i in subItemsWidth){
                    children[i].width = subItemsWidth[i] + itemsSpacing
                }
            }
        }
    }

    ListView{
        id: configNodes

        clip: true
        spacing: 5
        model: listModel
        visible: isExpand
        implicitHeight: rowAddRemove.width
        implicitWidth: rowAddRemove.height
        contentX: hbar.position * contentWidth

        property bool isItemCreated: false
        property int itemWidth: 0
        property int itemHeight: 0
        property bool isResizeTitle: false

        function resize()
        {
            if(listModel.count == 0){
                implicitWidth = rowAddRemove.width
                implicitHeight = rowAddRemove.height
            }else{
                if(configNodes.isItemCreated){
                    contentWidth = configNodes.itemWidth + 10
                    contentHeight = (configNodes.itemHeight + spacing) * listModel.count + 5

                    if(contentWidth > listMaxWidth){
                        implicitWidth = listMaxWidth
                    }else if(contentWidth < rowAddRemove.width){
                        implicitWidth = rowAddRemove.width
                    }else{
                        implicitWidth = contentWidth
                    }
                    if(contentHeight > listMaxHeight){
                        implicitHeight = listMaxHeight
                    }else if(contentHeight < rowAddRemove.height){
                        implicitHeight = rowAddRemove.height
                    }else{
                        implicitHeight = contentHeight
                    }
                }
            }
            rectHScrollBar.width = implicitWidth
        }

        delegate: Rectangle{
            id: rect
            color: configNodes.currentIndex == rowIndex ? "darkgray" : "transparent"

            Loader{
                source: self.isObjectArray ? "ObjectArrayRow.qml" : "SimpleTypeArrayRow.qml"
                onLoaded: {
                    item.rowModel = listModel.get(rowIndex)
                    item.index = rowIndex
                    if(self.isObjectArray){
                        item.configModel = rowData
                        item.init(self.configNames)
                    }else{
                        item.configModel = configModel
                        item.init(self.elementType)
                    }

                    rect.width = item.width
                    rect.height = item.height

                    if(!configNodes.isItemCreated){
                        configNodes.itemWidth = rect.width
                        configNodes.itemHeight = rect.height
                        configNodes.isItemCreated = true
                    }
                    configNodes.resize()
                    if(self.isObjectArray && !configNodes.isResizeTitle){
                        titleRow.resize(item)
                        configNodes.isResizeTitle = true
                    }
                }
            }

            MouseArea{
                anchors.fill: parent
                propagateComposedEvents: true
                onPressed: {
                    configNodes.currentIndex = rowIndex
                    mouse.accepted = false
                }
                onReleased: {
                    mouse.accepted = false
                }
            }
        }

        ScrollBar.vertical: ScrollBar{
        }
    }

    Rectangle{
        id: rectHScrollBar
        height: 15
        width: 200
        visible: isExpand
        color: "transparent"

        ScrollBar{
            id: hbar
            hoverEnabled: true
            active: hovered || pressed
            orientation: Qt.Horizontal
            size: configNodes.implicitWidth/configNodes.contentWidth
            anchors.fill: parent
        }
    }
}
