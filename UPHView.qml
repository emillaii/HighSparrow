import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
ColumnLayout {

    Button
    {
        text: qsTr("清零")
        onClicked: {
            lutModule.clearNumber()
            sensorLoaderModule.clearNumber()
        }
    }

    GroupBox
    {
        title:"左工位"
         ColumnLayout
         {
             GroupBox{
                 visible: baseModuleManager.getServerMode() === 0
                 title:"Lens UPH"
                 ColumnLayout{
                     RowLayout{
                         Label{
                             text:qsTr("lens总数")
                         }
                         TextField {
                             readOnly: true
                             text: left_lens_uph.currentNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("Ng lens数")
                         }
                         TextField {
                             readOnly: true
                             text: left_lens_uph.currentResultNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }

                     RowLayout{
                         Label{
                             text:qsTr("lens良率")
                         }
                         TextField {
                             readOnly: true
                             text: left_lens_uph.accumulatedYeild
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("lens UPH")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: left_lens_uph.accumulatedUPH
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                 }
             }
             GroupBox{
                 visible: baseModuleManager.getServerMode() === 1
                 title:"Sensor UPH"
                 ColumnLayout{
                     RowLayout{
                         Label{
                             text:qsTr("sensor总数")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: left_sensor_uph.currentNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("Ng sensor数")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: left_sensor_uph.currentResultNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                     RowLayout{
                         Label{
                             text:qsTr("sensor良率")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: left_sensor_uph.accumulatedYeild
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("sensor UPH")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: left_sensor_uph.accumulatedUPH
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                 }
             }
             GroupBox{
                 visible: baseModuleManager.getServerMode() === 1
                 title:"成品 UPH"
                 ColumnLayout{
                     RowLayout{
                         Label{
                             text:qsTr("成品总数")
                         }
                         TextField {
                             readOnly: true
                             text: left_product_uph.currentNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("综合良率")
                         }
                         TextField {
                             readOnly: true
                             text: left_comprehensive_uph.accumulatedYeild
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("成品 UPH")
                         }
                         TextField {
                             readOnly: true
                             text: left_product_uph.accumulatedUPH
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                 }
             }
         }
    }

    GroupBox
    {
        title:"右工位"
         ColumnLayout
         {
             GroupBox{
                 visible: baseModuleManager.getServerMode() === 0
                 title:"Lens UPH"
                 ColumnLayout{
                     RowLayout{
                         Label{
                             text:qsTr("lens总数")
                         }
                         TextField {
                             readOnly: true
                             text: right_lens_uph.currentNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("Ng lens数")
                         }
                         TextField {
                             readOnly: true
                             text: right_lens_uph.currentResultNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }

                     RowLayout{
                         Label{
                             text:qsTr("lens良率")
                         }
                         TextField {
                             readOnly: true
                             text: right_lens_uph.accumulatedYeild
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("lens UPH")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: right_lens_uph.accumulatedUPH
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                 }
             }
             GroupBox{
                 visible: baseModuleManager.getServerMode() === 1
                 title:"Sensor UPH"
                 ColumnLayout{
                     RowLayout{
                         Label{
                             text:qsTr("sensor总数")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: right_sensor_uph.currentNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("Ng sensor数")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: right_sensor_uph.currentResultNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                     RowLayout{
                         Label{
                             text:qsTr("sensor良率")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: right_sensor_uph.accumulatedYeild
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("sensor UPH")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: right_sensor_uph.accumulatedUPH
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                 }
             }
             GroupBox{
                 visible: baseModuleManager.getServerMode() === 1
                 title:"成品 UPH"
                 ColumnLayout{
                     RowLayout{
                         Label{
                             text:qsTr("成品总数")
                         }
                         TextField {
                             readOnly: true
                             text: right_product_uph.currentNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("综合良率")
                         }
                         TextField {
                             readOnly: true
                             text: right_comprehensive_uph.accumulatedYeild
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("成品 UPH")
                         }
                         TextField {
                             readOnly: true
                             text: right_product_uph.accumulatedUPH
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                 }
             }
         }
    }

    GroupBox
    {
        title:"总体"
         ColumnLayout
         {
             GroupBox{
                 visible: baseModuleManager.getServerMode() === 0
                 title:"Lens UPH"
                 ColumnLayout{
                     RowLayout{
                         Label{
                             text:qsTr("lens总数")
                         }
                         TextField {
                             readOnly: true
                             text: lens_uph.currentNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("Ng lens数")
                         }
                         TextField {
                             readOnly: true
                             text: lens_uph.currentResultNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }

                     RowLayout{
                         Label{
                             text:qsTr("lens良率")
                         }
                         TextField {
                             readOnly: true
                             text: lens_uph.accumulatedYeild
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("lens UPH")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: lens_uph.accumulatedUPH
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                 }
             }
             GroupBox{
                 visible: baseModuleManager.getServerMode() === 1
                 title:"Sensor UPH"
                 ColumnLayout{
                     RowLayout{
                         Label{
                             text:qsTr("sensor总数")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: sensor_uph.currentNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("Ng sensor数")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: sensor_uph.currentResultNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                     RowLayout{
                         Label{
                             text:qsTr("sensor良率")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: sensor_uph.accumulatedYeild
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("sensor UPH")
                             width: 150
                         }
                         TextField {
                             readOnly: true
                             text: sensor_uph.accumulatedUPH
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                 }
             }
             GroupBox{
                 visible: baseModuleManager.getServerMode() === 1
                 title:"成品 UPH"
                 ColumnLayout{
                     RowLayout{
                         Label{
                             text:qsTr("成品总数")
                         }
                         TextField {
                             readOnly: true
                             text: product_uph.currentNumber
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("综合良率")
                         }
                         TextField {
                             readOnly: true
                             text: comprehensive_uph.accumulatedYeild
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                         Label{
                             text:qsTr("成品 UPH")
                         }
                         TextField {
                             readOnly: true
                             text: product_uph.accumulatedUPH
                             horizontalAlignment: TextInput.AlignHCenter
                             width: 60
                         }
                     }
                 }
             }
         }
    }
}
