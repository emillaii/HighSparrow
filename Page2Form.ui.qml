import QtQuick 2.9
import QtQuick.Controls 2.2
import QtCharts 2.0
import QtQuick.Layouts 1.12
import QtCharts 2.0
Page {

    readonly property string downlookCameraName: "AA_DLCamera1"
    readonly property string uplookCameraName: "LUT_ULCamera"
    readonly property string pickarmCameraName: "PickarmCamera"

    id: page2
    width: 1280
    height: 720
    property alias frame: frame
    header: Label {
        text: qsTr("Page 2")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Row {
        id: row
        width: 900
        height: 300

        Frame {
            id: frame
            width: 300
            height: 300

            Image {
                id: prResultPreview1
                anchors.fill: parent
                source: ""
                fillMode: Image.PreserveAspectFit
                cache: false
                Connections {
                    target: visionModule
                    onCallQmlRefeshImg: {
                        element.text = Qt.formatTime(new Date(), "hh:mm:ss")
                        prResultPreview1.source = ""
                        prResultPreview1.source = "image://preview1/" + downlookCameraName
                    }
                }

                Text {
                    id: element
                    x: 126
                    y: 0
                    color: "#f2d03d"
                    text: qsTr("No Data")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    font.pixelSize: 20
                }
            }
        }

        Frame {
            id: frame1
            width: 300
            height: 300
        }

        Frame {
            id: frame2
            width: 300
            height: 300
        }
    }

    Connections {
        target: dataFromCpp
        onWValueChanged: {
            lrSeries.name = "Fuck u "
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

    ChartView {
        id: spline1
        title: "AA"
        antialiasing: true
        y: 306
        width: 608
        height: 420
        anchors.top: parent.top
        anchors.topMargin: 306
        anchors.left: parent.left
        anchors.leftMargin: 635
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 7
        theme: ChartView.ChartThemeBlueCerulean

        ValueAxis {
            id: axisY1
            titleText: "SFR"
            min: 0
            max: 100
            gridVisible: true
        }

        ValueAxis {
            id: axisX
            titleText: "Z (um)"
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
    }

    ChartView {
        id: spline
        width: 608
        height: 420
        anchors.top: parent.top
        anchors.topMargin: 306
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        title: "Spline"
        antialiasing: true
        theme: ChartView.ChartThemeBlueCerulean
        ValueAxis {
            id: axisY2
            min: 0
            max: 100
            gridVisible: false
            color: "#ffffff"
            labelsColor: "#ffffff"
            labelFormat: "%.0f"
        }

        SplineSeries {
            name: "SplineSeries"
            axisY: axisY2
            XYPoint {
                x: -500
                y: 1.622
            }
            XYPoint {
                x: -490
                y: 4.870392
            }
            XYPoint {
                x: -480
                y: 6.030605
            }
            XYPoint {
                x: -470
                y: 10.600190
            }
            XYPoint {
                x: -460
                y: 22.605797
            }
            XYPoint {
                x: -450
                y: 43.006839
            }
            XYPoint {
                x: -440
                y: 61.308766
            }
            XYPoint {
                x: -430
                y: 73.580432
            }
            XYPoint {
                x: -420
                y: 74.439490
            }
            XYPoint {
                x: -410
                y: 62.821909
            }
            XYPoint {
                x: -400
                y: 45.929113
            }
            XYPoint {
                x: -390
                y: 32.246433
            }
        }
    }
}
