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

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            RowLayout {
                Button {
                    id: button
                    text: qsTr("MTF Loop Test")
                    onClicked: {
                        logicManager.performLoopTest(4)
                    }
                }
            }
            RowLayout {
                ChartView {
                    id: spline1
                    anchors.fill: parent
                    title: "MTF"
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
                    Label {
                        id: spline1Label
                        text: "Any Supplmentary Data "
                        background: Rectangle {
                            color: "black"
                        }
                    }
                }
            }
        }
    }
}
