import QtQuick 2.9
import QtQuick.Controls 2.2
import QtCharts 2.0
import QtQuick.Layouts 1.11

Grid {
    id: grid
    width: 800
    height: 800

    Frame {
        id: frame
        width: grid.width
        height: grid.height

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            RowLayout {
                Button {
                    text: qsTr("MTF Loop Test")  //Vibration Test
                    onClicked: {
                        workersManager.startWorker("AACoreNew",true,3)
                        if (sensorLoderCheckBox.checked)
                            workersManager.startWorker("SensorLoader",true,3)
                    }
                }
                CheckBox {
                    id: sensorLoderCheckBox
                    checked: false
                    text: qsTr("Sensor Loader")
                }
            }
            RowLayout {
                ChartView {
                    id: spline1
                    anchors.fill: parent
                    title: "MTF"
                    antialiasing: true
                    theme: ChartView.ChartThemeDark

                    Connections {
                        target: dataFromCpp3
                        onWValueChanged: {
                            console.log(dataFromCpp3.wCCValue.x + " " + dataFromCpp3.wCCValue.y)
                            ccSeries.append(dataFromCpp3.wCCValue.x, dataFromCpp3.wCCValue.y)
                            ulSeries.append(dataFromCpp3.wULValue.x, dataFromCpp3.wULValue.y)
                            urSeries.append(dataFromCpp3.wURValue.x, dataFromCpp3.wURValue.y)
                            llSeries.append(dataFromCpp3.wLLValue.x, dataFromCpp3.wLLValue.y)
                            lrSeries.append(dataFromCpp3.wLRValue.x, dataFromCpp3.wLRValue.y)
                            axisX.min = ccSeries.at(0).x
                            axisX.max = ccSeries.at(ccSeries.count - 1).x
                        }
                        onWValueClear: {
                            ccSeries.clear()
                            ulSeries.clear()
                            urSeries.clear()
                            llSeries.clear()
                            lrSeries.clear()
                        }
                    }

                    ValueAxis {
                        id: axisY1
                        titleText: "SFR"
                        min: 0
                        max: 100
                        gridVisible: true
                    }

                    ValueAxis {
                        titleText: "Index"
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
