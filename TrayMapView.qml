import QtQuick 2.12
import QtQuick.Controls 2.2
import HighSprrow.Models 1.0

Item {
    Row {
        id: row
        anchors.fill: parent
        spacing: 5

        Frame{
            id: lensTrayRect
            width: parent.width / 4
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5

            Text{
                id: lensTrayTitle
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#ffffff"
                font.pixelSize: 20
                text: qsTr("Lens Tray Map")
            }

            TableView {
                id: lensTrayView
                anchors.top:lensTrayTitle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 5
                anchors.topMargin: 10
                rowSpacing: 3
                columnSpacing: 3
                clip: true

                contentX: contentWidth < width ? (contentWidth-width) / 2 : 0
                contentY: contentHeight < height ? (contentHeight-height) / 2 : 0
                leftMargin: contentWidth < width ? (width - contentWidth) / 2 : 0
                topMargin: contentHeight < height ? (height - contentHeight) / 2 : 0

                model: lensTrayModel

                delegate: Rectangle {
                    id: lensCell
                    implicitWidth: 40
                    implicitHeight: 40

                    color:
                    {
                        if(Status === TrayMapModel.StatusGood) // good
                        {
                            "#00ff00"
                        }
                        else if(Status === TrayMapModel.StatusNG) //NG
                        {
                            "#ff0000"
                        }
                        else if(Status === TrayMapModel.StatusIdle)// idle
                        {
                            "#ffff00"
                        }
                    }

                    ToolTip.visible: lensMouseArea.containsMouse
                    ToolTip.text:
                    {
                        var idx = row * lensTrayModel.columnCount() + column + 1
                        if(Status == TrayMapModel.StatusGood)
                        {
                            qsTr("Status: Good\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                        else if(Status == TrayMapModel.StatusNG)
                        {
                            qsTr("Status: NG\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                        else if(Status == TrayMapModel.StatusIdle)
                        {
                            qsTr("Status: Idle\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                    }

                    MouseArea
                    {
                        id: lensMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }
            }
        }

        Frame{
            id: sensorTrayRect
            width: parent.width / 4
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5

            Text{
                id: sensorTrayTitle
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#ffffff"
                font.pixelSize: 20
                text: qsTr("Sensor Tray Map")
            }

            TableView {
                id: sensorTrayView
                anchors.top:sensorTrayTitle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 5
                anchors.topMargin: 10
                rowSpacing: 3
                columnSpacing: 3
                clip: true

                contentX: contentWidth < width ? (contentWidth-width) / 2 : 0
                contentY: contentHeight < height ? (contentHeight-height) / 2 : 0
                leftMargin: contentWidth < width ? (width - contentWidth) / 2 : 0
                topMargin: contentHeight < height ? (height - contentHeight) / 2 : 0

                model: sensorTrayModel

                delegate: Rectangle {
                    id: sensorCell
                    implicitWidth: 40
                    implicitHeight: 40

                    color:
                    {
                        if(Status === TrayMapModel.StatusGood) // good
                        {
                            "#00ff00"
                        }
                        else if(Status === TrayMapModel.StatusNG) //NG
                        {
                            "#ff0000"
                        }
                        else if(Status === TrayMapModel.StatusIdle)// idle
                        {
                            "#ffff00"
                        }
                    }

                    ToolTip.visible: sensorMouseArea.containsMouse
                    ToolTip.text:
                    {
                        var idx = row * sensorTrayModel.columnCount() + column + 1
                        if(Status == TrayMapModel.StatusGood)
                        {
                            qsTr("Status: Good\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                        else if(Status == TrayMapModel.StatusNG)
                        {
                            qsTr("Status: NG\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                        else if(Status == TrayMapModel.StatusIdle)
                        {
                            qsTr("Status: Idle\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                    }

                    MouseArea
                    {
                        id: sensorMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }
            }
        }

        Frame{
            id: productTrayRect
            width: parent.width / 4
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5

            Text{
                id: productTrayTitle
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#ffffff"
                font.pixelSize: 20
                text: qsTr("Product Tray Map")
            }

            TableView {
                id: productTrayView
                anchors.top:productTrayTitle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 5
                anchors.topMargin: 10
                rowSpacing: 3
                columnSpacing: 3
                clip: true

                contentX: contentWidth < width ? (contentWidth-width) / 2 : 0
                contentY: contentHeight < height ? (contentHeight-height) / 2 : 0
                leftMargin: contentWidth < width ? (width - contentWidth) / 2 : 0
                topMargin: contentHeight < height ? (height - contentHeight) / 2 : 0

                model: productTrayModel

                delegate: Rectangle {
                    id: productCell
                    implicitWidth: 40
                    implicitHeight: 40

                    color:
                    {
                        if(Status === TrayMapModel.StatusGood) // good
                        {
                            "#00ff00"
                        }
                        else if(Status === TrayMapModel.StatusNG) //NG
                        {
                            "#ff0000"
                        }
                        else if(Status === TrayMapModel.StatusIdle)// idle
                        {
                            "#ffff00"
                        }
                    }

                    ToolTip.visible: productMouseArea.containsMouse
                    ToolTip.text:
                    {
                        var idx = row * productTrayModel.columnCount() + column + 1
                        if(Status == TrayMapModel.StatusGood)
                        {
                            qsTr("Status: Good\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                        else if(Status == TrayMapModel.StatusNG)
                        {
                            qsTr("Status: NG\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                        else if(Status == TrayMapModel.StatusIdle)
                        {
                            qsTr("Status: Idle\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                    }

                    MouseArea
                    {
                        id: productMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }
            }
        }

        Frame{
            id: rejectTrayRect
            width: parent.width / 4
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5

            Text{
                id: rejectTrayTitle
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#ffffff"
                font.pixelSize: 20
                text: qsTr("Reject Tray Map")
            }

            TableView {
                id: rejectTrayView
                anchors.top:rejectTrayTitle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 5
                anchors.topMargin: 10
                rowSpacing: 3
                columnSpacing: 3
                clip: true

                contentX: contentWidth < width ? (contentWidth-width) / 2 : 0
                contentY: contentHeight < height ? (contentHeight-height) / 2 : 0
                leftMargin: contentWidth < width ? (width - contentWidth) / 2 : 0
                topMargin: contentHeight < height ? (height - contentHeight) / 2 : 0

                model: rejectTrayModel

                delegate: Rectangle {
                    id: rejectCell
                    implicitWidth: 40
                    implicitHeight: 40

                    color:
                    {
                        if(Status === TrayMapModel.StatusGood) // good
                        {
                            "#00ff00"
                        }
                        else if(Status === TrayMapModel.StatusNG) //NG
                        {
                            "#ff0000"
                        }
                        else if(Status === TrayMapModel.StatusIdle)// idle
                        {
                            "#ffff00"
                        }
                    }

                    ToolTip.visible: rejectMouseArea.containsMouse
                    ToolTip.text:
                    {
                        var idx = row * rejectTrayModel.columnCount() + column + 1
                        if(Status == TrayMapModel.StatusGood)
                        {
                            qsTr("Status: Good\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                        else if(Status == TrayMapModel.StatusNG)
                        {
                            qsTr("Status: NG\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                        else if(Status == TrayMapModel.StatusIdle)
                        {
                            qsTr("Status: Idle\r\nUnit %1, Row %2, Column %3").arg(idx).arg(row+1).arg(column+1)
                        }
                    }

                    MouseArea
                    {
                        id: rejectMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }
            }
        }
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
