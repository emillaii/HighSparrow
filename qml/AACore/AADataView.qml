import QtQuick 2.9
import QtQuick.Controls 2.2
import QtCharts 2.0
import QtQuick.Layouts 1.11
Grid {
    id: grid
    columns: 4
    width: 800
    height: 800

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
                id: lrSeries
                axisX: axisX
                axisY: axisY1
            }
            SplineSeries {
                name: "LR "
                id: llSeries
                axisX: axisX
                axisY: axisY1
            }
            ColumnLayout {
                Label {
                    id: spline1Label
                    background: Rectangle {
                        color: "black"
                    }
                }
                Label {
                    id: spline1Label2
                    background: Rectangle {
                        color: "black"
                    }
                }
                Label {
                    id: spline1Label3
                    background: Rectangle {
                        color: "black"
                    }
                }
                Label {
                    id: spline1Label4
                    background: Rectangle {
                        color: "black"
                    }
                }
            }
        }

        Connections {
            target: dataFromCpp
            onWValueChanged: {
                spline1Label.text = dataFromCpp.layer0
                spline1Label2.text = dataFromCpp.layer1
                spline1Label3.text = dataFromCpp.layer2
                spline1Label4.text = dataFromCpp.layer3
                ccSeries.append(dataFromCpp.wCCValue.x, dataFromCpp.wCCValue.y)
                ulSeries.append(dataFromCpp.wULValue.x, dataFromCpp.wULValue.y)
                urSeries.append(dataFromCpp.wURValue.x, dataFromCpp.wURValue.y)
                llSeries.append(dataFromCpp.wLLValue.x, dataFromCpp.wLLValue.y)
                lrSeries.append(dataFromCpp.wLRValue.x, dataFromCpp.wLRValue.y)
                zPeak.append(dataFromCpp.zPeak,0)
                zPeak.append(dataFromCpp.zPeak,60)
                axisX.min = ccSeries.at(0).x
                axisX.max = ccSeries.at(ccSeries.count - 1).x
            }
            onWValueClear: {
                console.log("Clear")
                ccSeries.clear()
                ulSeries.clear()
                urSeries.clear()
                llSeries.clear()
                lrSeries.clear()
                zPeak.clear()
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
                max: 2000
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
                iSeriesLabel.text = "Min: " + dataFromIntensityProfile.minValue + " Max: " + dataFromIntensityProfile.maxValue
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
                id: lrSeries2
                axisX: axisX2
                axisY: axisY2
            }
            SplineSeries {
                name: "LR "
                id: llSeries2
                axisX: axisX2
                axisY: axisY2
            }
            ColumnLayout {
                Label {
                    id: spline2Label
                    background: Rectangle {
                        color: "black"
                    }
                }
                Label {
                    id: spline2Label2
                    background: Rectangle {
                        color: "black"
                    }
                }
                Label {
                    id: spline2Label3
                    background: Rectangle {
                        color: "black"
                    }
                }
                Label {
                    id: spline2Label4
                    background: Rectangle {
                        color: "black"
                    }
                }
            }
        }

        Connections {
            target: dataFromCpp2
            onWValueChanged: {
                spline2Label.text = dataFromCpp2.layer0
                spline2Label2.text = dataFromCpp2.layer1
                spline2Label3.text = dataFromCpp2.layer2
                spline2Label4.text = dataFromCpp2.layer3
                ccSeries2.append(dataFromCpp2.wCCValue.x, dataFromCpp2.wCCValue.y)
                ulSeries2.append(dataFromCpp2.wULValue.x, dataFromCpp2.wULValue.y)
                urSeries2.append(dataFromCpp2.wURValue.x, dataFromCpp2.wURValue.y)
                llSeries2.append(dataFromCpp2.wLLValue.x, dataFromCpp2.wLLValue.y)
                lrSeries2.append(dataFromCpp2.wLRValue.x, dataFromCpp2.wLRValue.y)
                zPeak2.append(dataFromCpp2.zPeak,0)
                zPeak2.append(dataFromCpp2.zPeak,60)
                axisX2.min = ccSeries2.at(0).x
                axisX2.max = ccSeries2.at(ccSeries2.count - 1).x
            }
            onWValueClear: {
                console.log("Clear")
                ccSeries2.clear()
                ulSeries2.clear()
                urSeries2.clear()
                llSeries2.clear()
                lrSeries2.clear()
                zPeak2.clear()
            }
        }
    }
}
