import QtQuick 2.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.11
import QtQuick 2.11

Popup {
    property double selectedStepSize: 0.1
    id: dialog
    visible: false
    contentItem: Rectangle {
        id: rectangle
        color: "black"
        implicitWidth: 900
        implicitHeight: 600
        RowLayout {
            Timer {
                id:timer
                interval: 500; running: true; repeat: true
            }
            ScrollView {
                id:output_sv
                implicitWidth:  200
                implicitHeight: 600
                clip: true
                ColumnLayout{
                    Switch {
                        id:special_ouput
                        text: qsTr("SUT Presser")
                        Connections{
                            target: timer
                            onTriggered: {
                                if (dialog.visible) {
                                    if(special_ouput.checked !== baseModuleManager.getOutput("POGOPIN下"))
                                        special_ouput.toggle()
                                    //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                }
                            }
                        }
                        onCheckedChanged:{
                            baseModuleManager.setOutput("POGOPIN下", checked)
                            baseModuleManager.setOutput("POGOPIN上", !checked)
                        }
                    }
                    ListView{
                        model: outputList
                        height: 50*count
                        //anchors.top: special_ouput.bottom
                        delegate: Switch{
                            id:output_switch_id
                            Connections{
                                target: timer
                                onTriggered: {
                                    if (dialog.visible) {
                                        if(output_switch_id.checked !== baseModuleManager.getOutput(modelData)){
                                            output_switch_id.toggle()
                                            console.log("22"+modelData+output_switch_id.checked)
                                        }

                                        //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                    }
                                }
                            }
                            text:qsTr(modelData)
                            onCheckedChanged: {
                                baseModuleManager.setOutput(modelData,checked);
                                console.log(modelData+checked)
                            }
                        }
                    }
                }
            }
            ScrollView{
                id:input_sv
                //anchors.left:output_sv.right
                implicitWidth:  200
                implicitHeight: 600
                clip: true
                ListView{
                    model:inputList
                    anchors.fill: parent
                    delegate: Switch{
                        id:input_swich_id
                        Connections{
                            target: timer
                            onTriggered: {
                                if (dialog.visible) {
                                    if(input_swich_id.checked !== baseModuleManager.getInput(modelData))
                                        input_swich_id.toggle()
                                    //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                }
                            }
                        }
                        text:qsTr(modelData)
                        onCheckableChanged: {
                            //baseModuleManager.setOutput(modelData,checked);
                        }
                    }
                }
            }
            ScrollView{
                implicitWidth:  200
                implicitHeight: 600
                clip: true
                ListView{
                    id:vacuum_list
                    model:vacuums_list
                    delegate:GroupBox {
                        title:vacuumName
                        ColumnLayout{
                            Switch{
                                id:break_io_switch
                                enabled: breakIoName!=""
                                Connections{
                                    target: timer
                                    onTriggered:{
                                        if (dialog.visible) {
                                            if(break_io_switch.checked !== baseModuleManager.getOutput(breakIoName)){
                                                break_io_switch.toggle()
                                                console.log("22"+breakIoName+break_io_switch.checked)
                                            }
                                        }
                                    }
                                }
                                onCheckedChanged: {
                                    baseModuleManager.setOutput(breakIoName,checked)
                                }

                                text:breakIoName!=""?breakIoName:"无breakIO"
                            }
                            Switch{
                                anchors.top: break_io_switch.bottom
                                id:in_io_switch
                                enabled: inIoName!=""
                                Connections{
                                    target: timer
                                    onTriggered:{
                                        if (dialog.visible) {
                                            if(in_io_switch.checked !== baseModuleManager.getInput(inIoName)){
                                                in_io_switch.toggle()
                                                console.log("22"+inIoName+in_io_switch.checked)
                                            }
                                        }
                                    }
                                }
                                text:inIoName!=""?inIoName:"无inIO"
                            }
                            Switch{
                                anchors.top: in_io_switch.bottom
                                id:out_io_switch
                                enabled: outIoName!=""
                                Connections{
                                    target: timer
                                    onTriggered:{
                                        if (dialog.visible) {
                                            if(out_io_switch.checked !== baseModuleManager.getOutput(outIoName)){
                                                out_io_switch.toggle()
                                                console.log("22"+outIoName+out_io_switch.checked)
                                            }
                                        }
                                    }
                                }
                                onCheckedChanged: {
                                    baseModuleManager.setOutput(outIoName,checked)
                                }
                                text:outIoName!=""?outIoName:"无outIO"
                            }
                        }
                    }
                }
            }
            ScrollView{
                implicitWidth:  300
                implicitHeight: 600
                clip: true
                ListView{
                    id:cylinder_list
                    model: cylinders_list
                    delegate:GroupBox {
                        title:cylinderName
                        ColumnLayout{
                            Switch{
                                id:one_in
                                enabled: oneInName!=""
                                Connections{
                                    target: timer
                                    onTriggered:{
                                        if (dialog.visible) {
                                            if(one_in.checked !== baseModuleManager.getInput(oneInName)){
                                                one_in.toggle()
                                                console.log("22"+oneInName+one_in.checked)
                                            }
                                        }
                                    }
                                }
                                text:oneInName!=""?oneInName:"无oneInIO"
                            }
                            Switch{
                                anchors.top: one_in.bottom
                                id:one_out
                                enabled: oneOutName!=""
                                Connections{
                                    target: timer
                                    onTriggered:{
                                        if (dialog.visible) {
                                            if(one_out.checked !== baseModuleManager.getOutput(oneOutName)){
                                                one_out.toggle()
                                                console.log("22"+oneOutName+one_out.checked)
                                            }
                                        }
                                    }
                                }
                                text:oneOutName!=""?oneOutName:"无oneOutIO"
                                onCheckedChanged: {
                                    baseModuleManager.setOutput(oneOutName,checked)
                                }
                            }
                            Switch{
                                anchors.top: one_out.bottom
                                id:zero_in
                                enabled: zeroInName!=""
                                Connections{
                                    target: timer
                                    onTriggered:{
                                        if (dialog.visible) {
                                            if(zero_in.checked !== baseModuleManager.getInput(zeroInName)){
                                                zero_in.toggle()
                                                console.log("22"+zeroInName+zero_in.checked)
                                            }
                                        }
                                    }
                                }
                                text:zeroInName!=""?zeroInName:"无outIO"
                            }
                            Switch{
                                anchors.top: zero_in.bottom
                                id:zero_out
                                enabled: zeroOutName!=""
                                Connections{
                                    target: timer
                                    onTriggered:{
                                        if (dialog.visible) {
                                            if(zero_out.checked !== baseModuleManager.getOutput(zeroOutName)){
                                                zero_out.toggle()
                                                console.log("22"+zeroOutName+zero_out.checked)
                                            }
                                        }
                                    }
                                }
                                text:zeroOutName!=""?zeroOutName:"无oneOutIO"
                                onCheckedChanged: {
                                    baseModuleManager.setOutput(zeroOutName,checked)
                                }
                            }
                        }
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
