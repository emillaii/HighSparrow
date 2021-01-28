import QtQuick 2.9
import QtQuick.Controls 2.2
import QtCharts 2.0
import QtQuick.Layouts 1.11

Grid {
    id: grid
    columns: 4
    width: 800
    height: 800

    //ToDo: Refactor the plotChart1 and plotChart2
    function plotChart1() {
        spline1Label.text = dataFromCpp.layer0
        spline1Label2.text = dataFromCpp.layer1
        spline1Label3.text = dataFromCpp.layer2
        spline1Label4.text = dataFromCpp.layer3
        spline1.title = dataFromCpp.chartName
        var maxSfr = 0
        if (displaySpline1Mode.checked) {
            for (var i=0; i<dataFromCpp.wCCList.length; i=i+2) {
                ccSeries.append(dataFromCpp.wCCList[i], dataFromCpp.wCCList[i+1])
                if (dataFromCpp.wCCList[i+1] > maxSfr) {
                    maxSfr = dataFromCpp.wCCList[i+1]
                }
            }
            for (i=0; i<dataFromCpp.wULList.length; i=i+2) {
                ulSeries.append(dataFromCpp.wULList[i], dataFromCpp.wULList[i+1])
            }
            for (i=0; i<dataFromCpp.wURList.length; i=i+2) {
                urSeries.append(dataFromCpp.wURList[i], dataFromCpp.wURList[i+1])
            }
            for (i=0; i<dataFromCpp.wLLList.length; i=i+2) {
                llSeries.append(dataFromCpp.wLLList[i], dataFromCpp.wLLList[i+1])
            }
            for (i=0; i<dataFromCpp.wLRList.length; i=i+2) {
                lrSeries.append(dataFromCpp.wLRList[i], dataFromCpp.wLRList[i+1])
            }
        } else {
            for (var i=0; i<dataFromCpp.wCCRealList.length; i=i+2) {
                ccSeries.append(dataFromCpp.wCCRealList[i], dataFromCpp.wCCRealList[i+1])
                if (dataFromCpp.wCCRealList[i+1] > maxSfr) {
                    maxSfr = dataFromCpp.wCCRealList[i+1]
                }
            }
            for (i=0; i<dataFromCpp.wULRealList.length; i=i+2) {
                ulSeries.append(dataFromCpp.wULRealList[i], dataFromCpp.wULRealList[i+1])
            }
            for (i=0; i<dataFromCpp.wURRealList.length; i=i+2) {
                urSeries.append(dataFromCpp.wURRealList[i], dataFromCpp.wURRealList[i+1])
            }
            for (i=0; i<dataFromCpp.wLLRealList.length; i=i+2) {
                llSeries.append(dataFromCpp.wLLRealList[i], dataFromCpp.wLLRealList[i+1])
            }
            for (i=0; i<dataFromCpp.wLRRealList.length; i=i+2) {
                lrSeries.append(dataFromCpp.wLRRealList[i], dataFromCpp.wLRRealList[i+1])
            }
        }

        axisX.min = ccSeries.at(0).x
        axisX.max = ccSeries.at(ccSeries.count - 1).x
        if (maxSfr > 80) {
            axisY1.max = 100
            zPeak.append(dataFromCpp.zPeak,0)
            zPeak.append(dataFromCpp.zPeak,100)
        } else if (maxSfr > 60) {
            axisY1.max = 80
            zPeak.append(dataFromCpp.zPeak,0)
            zPeak.append(dataFromCpp.zPeak,80)
        } else {
            axisY1.max = 60
            zPeak.append(dataFromCpp.zPeak,0)
            zPeak.append(dataFromCpp.zPeak,60)
        }
    }

    function plotChart2() {
        spline2Label.text = dataFromCpp2.layer0
        spline2Label2.text = dataFromCpp2.layer1
        spline2Label3.text = dataFromCpp2.layer2
        spline2Label4.text = dataFromCpp2.layer3
        spline2.title = dataFromCpp2.chartName
        var maxSfr = 0

        if(displaySpline2Mode.checked) {
            for (var i=0; i<dataFromCpp2.wCCList.length; i=i+2) {
                ccSeries2.append(dataFromCpp2.wCCList[i], dataFromCpp2.wCCList[i+1])
                if (dataFromCpp2.wCCList[i+1] > maxSfr) {
                    maxSfr = dataFromCpp2.wCCList[i+1]
                }
            }
            for (i=0; i<dataFromCpp2.wULList.length; i=i+2) {
                ulSeries2.append(dataFromCpp2.wULList[i], dataFromCpp2.wULList[i+1])
            }
            for (i=0; i<dataFromCpp2.wURList.length; i=i+2) {
                urSeries2.append(dataFromCpp2.wURList[i], dataFromCpp2.wURList[i+1])
            }
            for (i=0; i<dataFromCpp2.wLLList.length; i=i+2) {
                llSeries2.append(dataFromCpp2.wLLList[i], dataFromCpp2.wLLList[i+1])
            }
            for (i=0; i<dataFromCpp2.wLRList.length; i=i+2) {
                lrSeries2.append(dataFromCpp2.wLRList[i], dataFromCpp2.wLRList[i+1])
            }
        } else {
            for (var i=0; i<dataFromCpp2.wCCRealList.length; i=i+2) {
                ccSeries2.append(dataFromCpp2.wCCRealList[i], dataFromCpp2.wCCRealList[i+1])
                if (dataFromCpp2.wCCRealList[i+1] > maxSfr) {
                    maxSfr = dataFromCpp2.wCCRealList[i+1]
                }
            }
            for (i=0; i<dataFromCpp2.wULRealList.length; i=i+2) {
                ulSeries2.append(dataFromCpp2.wULRealList[i], dataFromCpp2.wULRealList[i+1])
            }
            for (i=0; i<dataFromCpp2.wURRealList.length; i=i+2) {
                urSeries2.append(dataFromCpp2.wURRealList[i], dataFromCpp2.wURRealList[i+1])
            }
            for (i=0; i<dataFromCpp2.wLLRealList.length; i=i+2) {
                llSeries2.append(dataFromCpp2.wLLRealList[i], dataFromCpp2.wLLRealList[i+1])
            }
            for (i=0; i<dataFromCpp2.wLRRealList.length; i=i+2) {
                lrSeries2.append(dataFromCpp2.wLRRealList[i], dataFromCpp2.wLRRealList[i+1])
            }
        }

        axisX2.min = ccSeries2.at(0).x
        axisX2.max = ccSeries2.at(ccSeries2.count - 1).x
        if (maxSfr > 80) {
            axisY2.max = 100
            zPeak2.append(dataFromCpp2.zPeak,0)
            zPeak2.append(dataFromCpp2.zPeak,100)
        } else if (maxSfr > 60) {
            axisY2.max = 80
            zPeak2.append(dataFromCpp2.zPeak,0)
            zPeak2.append(dataFromCpp2.zPeak,80)
        } else {
            axisY2.max = 60
            zPeak2.append(dataFromCpp2.zPeak,0)
            zPeak2.append(dataFromCpp2.zPeak,60)
        }
    }

    function clearPlot1(){
        ccSeries.clear()
        ulSeries.clear()
        urSeries.clear()
        llSeries.clear()
        lrSeries.clear()
        zPeak.clear()
    }

    function clearPlot2(){
        ccSeries2.clear()
        ulSeries2.clear()
        urSeries2.clear()
        llSeries2.clear()
        lrSeries2.clear()
        zPeak2.clear()
    }

    Frame {
        id: frame
        width: grid.width/4
        height: grid.height/2

        Image {
            id: image1
            anchors.fill: parent
            source: "../../icons/sparrow.png"
            fillMode: Image.PreserveAspectFit
            cache: false
            Connections {
                target: highSprrow
                onDisplayOCImageInUI: {
                    image1.source = ""
                    image1.source = "image://ocImage1"
                }
            }
        }
    }

    Frame {
        id: frame1
        width: grid.width/4
        height: grid.height/2
        Image {
            id: image3
            anchors.fill: parent
            source: "../../icons/sparrow.png"
            fillMode: Image.PreserveAspectFit
            cache: false
            Rectangle {
                color: "pink"
                opacity: 0.8
                x: (image3.width - image3.paintedWidth)/2
                y: image3.height/2
                width: image3.paintedWidth
                height: 1
            }
            Rectangle {
                color: "pink"
                opacity: 0.8
                x: image3.width/2
                y: (image3.height - image3.paintedHeight)/2
                width: 1
                height: image3.paintedHeight
            }
            TextEdit {
                id: sensorIDLabel
                text: qsTr("SensorID: ") + dothinkey.currentSensorID
                color: "steelblue"
                font.bold: true
                font.italic: true
                font.underline: true
                cursorVisible: true
                mouseSelectionMode: TextEdit.SelectWords
                selectByMouse: true
//                background: Rectangle {
//                    color: "black"
//                }
            }
        }
        Connections {
            target: imageGrabberThread
            onCallQmlRefeshImg: {
                image3.source = ""
                image3.source = "image://imageGrabberLiveImage"
            }
        }
    }

    Frame {
        id: frame2
        width: grid.width/4
        height: grid.height/2

        ChartView {
            id: spline1
            anchors.fill: parent
            title: "AA "
            antialiasing: true
            theme: ChartView.ChartThemeDark
            legend.alignment: Qt.AlignBottom
            margins.top: 50
            RowLayout {
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                Switch {
                    id: displaySpline1Mode
                    font.pixelSize: 10
                    text: qsTr("Displaying Curve Data")
                    checked: true
                    onCheckedChanged: {
                        if (checked) {
                            text = qsTr("Displaying Curve Data")
                        } else {
                            text = qsTr("Displaying Raw Data")
                        }
                        clearPlot1()
                        plotChart1()
                    }
                }
                ProgressBar {
                    id: spline1progressBar
                    font.pointSize: 16
                    Layout.rightMargin: 7
                    Layout.fillWidth: true
                }
            }

            ValueAxis {
                id: axisY1
                titleText: "SFR"
                min: 0
                max: 60
                tickCount: 13
                gridVisible: true
            }

            ValueAxis {
                titleText: "z (um)"
                id: axisX
                tickCount: 7
                gridVisible: true
            }

            LineSeries
            {
                name:"zPeak"
                id:zPeak
                axisX: axisX
                axisY: axisY1
            }
            SplineSeries {
                name: "CC "
                id: ccSeries
                axisX: axisX
                axisY: axisY1
            }
            SplineSeries {
                name: "UL "
                id: ulSeries
                axisX: axisX
                axisY: axisY1
            }
            SplineSeries {
                name: "UR "
                id: urSeries
                axisX: axisX
                axisY: axisY1
            }
            SplineSeries {
                name: "LL "
                id: llSeries
                axisX: axisX
                axisY: axisY1
            }
            SplineSeries {
                name: "LR "
                id: lrSeries
                axisX: axisX
                axisY: axisY1
            }
            ColumnLayout {
                Label {
                    id: spline1Label
                    background: Rectangle {
                        color: "black"
                    }
                    font.pixelSize: 10
                }
                Label {
                    id: spline1Label2
                    background: Rectangle {
                        color: "black"
                    }
                    font.pixelSize: 10
                }
                Label {
                    id: spline1Label3
                    background: Rectangle {
                        color: "black"
                    }
                    font.pixelSize: 10
                }
                Label {
                    id: spline1Label4
                    background: Rectangle {
                        color: "black"
                    }
                    font.pixelSize: 10
                }
            }
        }

        Connections {
            target: dataFromCpp
            onWValueChanged: {
                 plotChart1()
            }
            onWValueClear: {
                clearPlot1()
            }
            onWInProgressChanged: {
                if (dataFromCpp.inProgress) {
                    spline1progressBar.indeterminate = true
                } else {
                    spline1progressBar.indeterminate = false
                }
            }
        }
    }

    Frame {
        id: frame3
        width: grid.width/4
        height: grid.height/2
        ChartView {
            id: lineSeries
            anchors.fill: parent
            antialiasing: true
            theme: ChartView.ChartThemeDark

            ValueAxis {
                id: axisY3
                titleText: ""
                min: 0
                max: 255
                gridVisible: true
            }

            ValueAxis {
                titleText: "pixel location "
                id: axisX3
                gridVisible: true
                min: 0
                max: dataFromIntensityProfile.valuesSize
            }

            LineSeries {
                name: "Intensity Profile "
                id: iSeries
                axisX: axisX3
                axisY: axisY3
            }
            ColumnLayout {
                Label {
                    id: iSeriesLabel
                    background: Rectangle {
                        color: "black"
                    }
                }
            }
        }
        Connections {
            target: dataFromIntensityProfile
            onWValueChanged: {
                iSeriesLabel.text = "Min: " + dataFromIntensityProfile.minValue + " Max: " + dataFromIntensityProfile.maxValue + " -veDI: " + dataFromIntensityProfile.negativeDI.toFixed(2) + " +veDI: " + dataFromIntensityProfile.positiveDI.toFixed(2)
                iSeries.append(dataFromIntensityProfile.wValue.x, dataFromIntensityProfile.wValue.y)
            }
            onWValueClear: {
                console.log("Clear")
                iSeries.clear()
            }
        }
    }

    Frame {
        id: frame4
        width: grid.width/4
        height: grid.height/2
        Image {
            id: image4
            anchors.fill: parent
            source: "../../icons/sparrow.png"
            fillMode: Image.PreserveAspectFit
            cache: false
            Connections {
                target: highSprrow
                onDisplaySfrImageInUI: {
                    image4.source = ""
                    image4.source = "image://sfrImage"
                }
            }
        }
    }

    Frame {
        id: frame6
        width: grid.width/4
        height: grid.height/2

        Image {
            id: image6
            anchors.fill: parent
            source: "../../icons/sparrow.png"
            fillMode: Image.PreserveAspectFit
            cache: false
            Connections {
                target: highSprrow
                onDisplayGlueInspectImageInUI: {
                    image6.source = ""
                    image6.source = "image://dispenseImage"
                }
            }
        }
    }

    Frame {
        id: frame5
        width: grid.width/4
        height: grid.height/2
        ChartView {
            id: spline2
            anchors.fill: parent
            title: "AA "
            antialiasing: true
            theme: ChartView.ChartThemeDark
            legend.alignment: Qt.AlignBottom
            margins.top: 50
            ValueAxis {
                id: axisY2
                titleText: "SFR"
                min: 0
                max: 60
                tickCount: 13
                gridVisible: true
            }

            ValueAxis {
                titleText: "z (um)"
                id: axisX2
                tickCount: 7
                gridVisible: true

           }
           LineSeries
           {
               name:"zPeak"
               id:zPeak2
               axisX: axisX2
               axisY: axisY2
           }
            SplineSeries {
                name: "CC "
                id: ccSeries2
                axisX: axisX2
                axisY: axisY2
            }
            SplineSeries {
                name: "UL "
                id: ulSeries2
                axisX: axisX2
                axisY: axisY2
            }
            SplineSeries {
                name: "UR "
                id: urSeries2
                axisX: axisX2
                axisY: axisY2
            }
            SplineSeries {
                name: "LL "
                id: llSeries2
                axisX: axisX2
                axisY: axisY2
            }
            SplineSeries {
                name: "LR "
                id: lrSeries2
                axisX: axisX2
                axisY: axisY2
            }
            RowLayout {
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                Switch {
                    id: displaySpline2Mode
                    font.pixelSize: 10
                    text: qsTr("Displaying Curve Data")
                    checked: true
                    onCheckedChanged: {
                        if (checked) {
                            text = qsTr("Displaying Curve Data")
                        } else {
                            text = qsTr("Displaying Raw Data")
                        }
                        clearPlot2()
                        plotChart2()
                    }
                }
                ProgressBar {
                    id: spline2progressBar
                    font.pointSize: 16
                    Layout.rightMargin: 7
                    Layout.fillWidth: true
                }
            }
            ColumnLayout {
                Label {
                    id: spline2Label
                    background: Rectangle {
                        color: "black"
                    }
                    font.pixelSize: 10
                }
                Label {
                    id: spline2Label2
                    background: Rectangle {
                        color: "black"
                    }
                    font.pixelSize: 10
                }
                Label {
                    id: spline2Label3
                    background: Rectangle {
                        color: "black"
                    }
                    font.pixelSize: 10
                }
                Label {
                    id: spline2Label4
                    background: Rectangle {
                        color: "black"
                    }
                    font.pixelSize: 10
                }
            }
        }

        Connections {
            target: dataFromCpp2
            onWValueChanged: {
                plotChart2()
            }
            onWValueClear: {
                clearPlot2()
            }
            onWInProgressChanged: {
                if (dataFromCpp2.inProgress) {
                    spline2progressBar.indeterminate = true
                } else {
                    spline2progressBar.indeterminate = false
                }
            }
        }
    }

    Frame {
        id: frame7
        width: grid.width/4
        height: grid.height/2

        ColumnLayout {
            RowLayout {
                Label {
                    text: qsTr("Current Cycle Time: ")
                    font.bold: true
                    font.italic: true
                }
                TextField {
                    readOnly: true
                    text: aaCoreParams.circleTime
                    horizontalAlignment: TextInput.AlignHCenter
                    width: 60
                }

                Button {
                    text: qsTr("reset")
                    onClicked: {
                        baseModuleManager.resetUPH()
                    }
                }
            }

            RowLayout {
                Label {
                    text: qsTr("Current UPH: ")
                    font.bold: true
                    font.italic: true
                }
                TextField {
                    readOnly: true
                    text: aaCoreParams.currentUPH
                    horizontalAlignment: TextInput.AlignHCenter
                    width: 60
                }
            }

            RowLayout {
                Label {
                    text: qsTr("Average Cycle Time: ")
                    font.bold: true
                    font.italic: true
                }
                TextField {
                    readOnly: true
                    text: aaCoreParams.circleAverageTime
                    horizontalAlignment: TextInput.AlignHCenter
                    width: 60
                }
            }

            RowLayout {
                Label {
                    text: qsTr("Gross Cycle Count: ")
                    font.bold: true
                    font.italic: true
                }
                TextField {
                    readOnly: true
                    text: aaCoreParams.circleCount
                    horizontalAlignment: TextInput.AlignHCenter
                    width: 60
                }
            }

            RowLayout {
                Label {
                    text: qsTr("Gross UPH: ")
                    font.bold: true
                    font.italic: true
                }
                TextField {
                    readOnly: true
                    text: aaCoreParams.calculatedUPH
                    horizontalAlignment: TextInput.AlignHCenter
                    width: 60
                }
            }

            RowLayout {
                Label {
                    text: qsTr("Dispense Count: ")
                    font.bold: true
                    font.italic: true
                }
                TextField {
                    readOnly: true
                    text: aaCoreParams.dispenseCount
                    horizontalAlignment: TextInput.AlignHCenter
                    width: 60
                }
                Label {
                    text: qsTr("/")
                    font.bold: true
                    font.italic: true
                }
                TextEdit {
                    color: "white"
                    width :100
                    text: aaCoreParams.dispenseCountLimit
                    readOnly: true
                }
                Button {
                    text: qsTr("Clear")
                    onClicked: {
                        console.log("Clear dispense count")
                        aaNewCore.clearCurrentDispenseCount()
                    }
                }
            }
        }
    }
}


