import QtQuick 2.9
import QtQuick.Controls 2.2
import QtCharts 2.0
import QtQuick.Layouts 1.11
Grid {
    id: grid
    columns: 3
    width: 800
    height: 800

    Frame {
        id: frame
        width: grid.width/3
        height: grid.height/2

        Image {
            id: image1
            anchors.fill: parent
            source: "icons/sparrow.png"
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
        width: grid.width/3
        height: grid.height/2
        Image {
            id: image3
            anchors.fill: parent
            source: "icons/sparrow.png"
            fillMode: Image.PreserveAspectFit
            cache: false
            Label {
                id: sensorIDLabel
                text: qsTr("SensorID: ") + dothinkey.currentSensorID
                background: Rectangle {
                    color: "black"
                }
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
        width: grid.width/3
        height: grid.height/2

        ChartView {
            id: spline1
            anchors.fill: parent
            title: "AA"
            antialiasing: true
            theme: ChartView.ChartThemeDark

            ValueAxis {
                id: axisY1
                titleText: "SFR"
                min: 0
                max: 100
                gridVisible: true
            }

            ValueAxis {
                titleText: "z (um)"
                id: axisX
                gridVisible: true
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
            }
        }

        Connections {
            target: dataFromCpp
            onWValueChanged: {
                spline1Label.text = "CC: " + dataFromCpp.wCCPeakZ + " UL: " + dataFromCpp.wULPeakZ +
                        " UR: " + dataFromCpp.wURPeakZ + " LL: " + dataFromCpp.wLLPeakZ +
                        " LR: " + dataFromCpp.wLRPeakZ + " DEV: " + dataFromCpp.dev
                spline1Label2.text = "XTilt: " + dataFromCpp.xTilt + " YTilt: " + dataFromCpp.yTilt
                ccSeries.append(dataFromCpp.wCCValue.x, dataFromCpp.wCCValue.y)
                ulSeries.append(dataFromCpp.wULValue.x, dataFromCpp.wULValue.y)
                urSeries.append(dataFromCpp.wURValue.x, dataFromCpp.wURValue.y)
                llSeries.append(dataFromCpp.wLLValue.x, dataFromCpp.wLLValue.y)
                lrSeries.append(dataFromCpp.wLRValue.x, dataFromCpp.wLRValue.y)
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
            }
        }
    }

    Frame {
        id: frame3
        width: grid.width/3
        height: grid.height/2
    }

    Frame {
        id: frame4
        width: grid.width/3
        height: grid.height/2
        Image {
            id: image4
            anchors.fill: parent
            source: "icons/sparrow.png"
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
        id: frame5
        width: grid.width/3
        height: grid.height/2
        ChartView {
            id: spline2
            anchors.fill: parent
            title: "AA"
            antialiasing: true
            theme: ChartView.ChartThemeDark

            ValueAxis {
                id: axisY2
                titleText: "SFR"
                min: 0
                max: 100
                gridVisible: true
            }

            ValueAxis {
                titleText: "z (um)"
                id: axisX2
                gridVisible: true
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
            }
        }

        Connections {
            target: dataFromCpp2
            onWValueChanged: {
                spline2Label.text = "CC: " + dataFromCpp2.wCCPeakZ + " UL: " + dataFromCpp2.wULPeakZ +
                        " UR: " + dataFromCpp2.wURPeakZ + " LL: " + dataFromCpp2.wLLPeakZ +
                        " LR: " + dataFromCpp2.wLRPeakZ + " DEV: " + dataFromCpp2.dev
                spline2Label2.text = "XTilt: " + dataFromCpp2.xTilt + " YTilt: " + dataFromCpp2.yTilt
                ccSeries2.name = "CC " + dataFromCpp2.wCCPeakZ
                ccSeries2.append(dataFromCpp2.wCCValue.x, dataFromCpp2.wCCValue.y)
                ulSeries2.append(dataFromCpp2.wULValue.x, dataFromCpp2.wULValue.y)
                urSeries2.append(dataFromCpp2.wURValue.x, dataFromCpp2.wURValue.y)
                llSeries2.append(dataFromCpp2.wLLValue.x, dataFromCpp2.wLLValue.y)
                lrSeries2.append(dataFromCpp2.wLRValue.x, dataFromCpp2.wLRValue.y)
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
            }
        }
    }
}
