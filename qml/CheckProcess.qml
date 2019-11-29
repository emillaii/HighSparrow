import QtQuick 2.9
//import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import FileContentItem 1.0
import QtQuick.Dialogs 1.2
import "AAHeadModule"
import "LUTModule"
import "SUTModule"
import SomeLib 1.1
import LogicManagerLib 1.1

ItemDelegate {
    width: parent.width

    contentItem: ColumnLayout{
        spacing: 0

        ColumnLayout {
            Timer {
                id:timer
                interval: 500; running: true; repeat: true
            }
            RowLayout {
                Label {
                    text: qsTr("")
                }
            }
            GroupBox{
                title:qsTr("校正流程")
                ColumnLayout{
                    Label{
                        text:qsTr("1.放入蘑菇头，确定LUT的 AA1UplookPosition和AA2UplookPosition及其等待位置")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process1
                        onClicked: {
                            process1_operate.visible =true;
                            btn_process1.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process1_operate
                        ColumnLayout{
                            GroupBox{
                                title: qsTr("AA_1 Uplook Position")
                                LUTAA1UplookPosition {
                                    RowLayout{
                                        Button{
                                            text:qsTr("校正")
                                            onClicked: {
                                                //logicManager.performCalibration("aa1_upLook_calibration")
                                                logicManagerState.setCurrentCalibrationName("aa1_uplook_calibration")
                                                logicManager.performHandling(LogicManager.PERFORM_CALIBRATION)
                                            }
                                        }
                                        Button{
                                            text:qsTr("执行PR")
                                            onClicked: {
                                                //logicManager.performLocation("aa1_upLook_location")
                                                logicManagerState.setCurrentLocationName("aa1_upLook_location")
                                                logicManagerState.setUseOriginPr(true)
                                                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                                            }
                                        }
                                    }
                                }
                            }
                            GroupBox{
                                title: qsTr("AA_2 Uplook Position")
                                LUTAA2UplookPosition {
                                    Button{
                                        text:qsTr("执行PR")
                                        onClicked: {
                                            //logicManager.performLocation("aa2_upLook_location")
                                            logicManagerState.setCurrentLocationName("aa2_upLook_location")
                                            logicManagerState.setUseOriginPr(true)
                                            logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                                        }
                                    }
                                }
                            }

                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process1_operate.visible =false;
                                    btn_process1.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("2.放置好Chart图、拿走蘑菇头")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process2
                        onClicked: {
                            process2_operate.visible =true;
                            btn_process2.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process2_operate
                        ColumnLayout{
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process2_operate.visible =false;
                                    btn_process2.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("3.移走给AA1、AA2手动夹上lens")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process3
                        onClicked: {
                            process3_operate.visible =true;
                            btn_process3.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process3_operate
                        ColumnLayout{
                            Switch {
                                id:aa1_gripon
                                text: qsTr("AA1_GripON")
                                Connections{
                                    target: timer
                                    onTriggered: {
                                        if (process3_operate.visible) {
                                            if(aa1_gripon.checked !== baseModuleManager.getOutput("AA1_GripON"))
                                                aa1_gripon.toggle()
                                            //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                        }
                                    }
                                }
                                onCheckedChanged:{
                                    baseModuleManager.setOutput("AA1_GripON", checked)
                                }
                            }

                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process3_operate.visible =false;
                                    btn_process3.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("4.确定AA头的大致的mushroom位置、高度以视觉清晰为准、大概水平")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process4
                        onClicked: {
                            process4_operate.visible =true;
                            btn_process4.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process4_operate
                        ColumnLayout{
                            GroupBox{
                                title: qsTr("Mushroom Position")
                                AAHeadMushroomPosition {}
                            }
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process4_operate.visible =false;
                                    btn_process4.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("5.提示SUT1、SUT2打水平并手动放上sensor并点亮")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process5
                        onClicked: {
                            process5_operate.visible =true;
                            btn_process5.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process5_operate
                        ColumnLayout{
                            Switch {
                                id:pogopin_ouput
                                text: qsTr("POGOPIN")
                                Connections{
                                    target: timer
                                    onTriggered: {
                                        if (process5_operate.visible) {
                                            if(pogopin_ouput.checked !== baseModuleManager.getOutput("POGOPIN_DOWN"))
                                                pogopin_ouput.toggle()
                                            //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                        }
                                    }
                                }
                                onCheckedChanged:{
                                    baseModuleManager.setOutput("POGOPIN_DOWN", checked)
                                    baseModuleManager.setOutput("POGOPIN_UP", !checked)
                                }
                            }
                            Switch {
                                id:sutvacuum_ouput
                                text: qsTr("SUT1吸真空")
                                Connections{
                                    target: timer
                                    onTriggered: {
                                        if (process5_operate.visible) {
                                            if(sutvacuum_ouput.checked !== baseModuleManager.getOutput("SUT1吸真空"))
                                                sutvacuum_ouput.toggle()
                                            //if(index%2==0&&!swich_id.checked)swich_id.toggle()
                                        }
                                    }
                                }
                                onCheckedChanged:{
                                    baseModuleManager.setOutput("SUT1吸真空", checked)
                                }
                            }
                            Button {
                                id: button
                                text: qsTr("点亮开关")
                                onClicked: {
                                    baseModuleManager.initSensor()
                                }
                            }
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process5_operate.visible =false;
                                    btn_process5.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("6.确定SUT的DownlookPR位置")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process6
                        onClicked: {
                            process6_operate.visible =true;
                            btn_process6.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process6_operate
                        ColumnLayout{
                            GroupBox{
                                title: qsTr("PR Position")
                                SUTPRPositionView {}
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("校正")
                                    onClicked: {

                                    }
                                }
                                Button{
                                    text:qsTr("执行PR")
                                    onClicked: {
                                        sutModule.moveToDownlookPR()
                                    }
                                }
                            }

                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process6_operate.visible =false;
                                    btn_process6.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("7.确定SUT与LUT的位置关系")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process7
                        onClicked: {
                            process7_operate.visible =true;
                            btn_process7.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process7_operate
                        ColumnLayout{
                            RowLayout{
                                Button{
                                    text:qsTr("LUT 退到指定位置")
                                    onClicked:{

                                    }
                                }
                                Button{
                                    text:qsTr("校正SUT UpdownlookDownCalibration")
                                    onClicked: {
                                        //logicManager.performCalibration("aa1_updownLook_down_calibration")
                                        logicManagerState.setCurrentCalibrationName("aa1_updownLook_down_calibration")
                                        logicManager.performHandling(LogicManager.PERFORM_CALIBRATION)
                                    }
                                }

                            }
                            RowLayout{
                                Button{
                                    text:qsTr("校正SUT UpdownLookUpCailration")
                                    onClicked: {
                                        //logicManager.performCalibration("aa1_updownLook_up_calibration")
                                        logicManagerState.setCurrentCalibrationName("aa1_updownLook_up_calibration")
                                        logicManager.performHandling(LogicManager.PERFORM_CALIBRATION)
                                    }
                                }
                                Button{
                                    text:qsTr("执行SUT UpDownlookPR")
                                    onClicked: {
                                        sutModule.toolUplookPR()
                                    }
                                }
                            }

                            GroupBox{
                                title: qsTr("Mushroom Position")
                                SUTMushroomPosition {}
                            }
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process7_operate.visible =false;
                                    btn_process7.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("8.做OC、AA ")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process8
                        onClicked: {
                            process8_operate.visible =true;
                            btn_process8.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process8_operate
                        ColumnLayout{
                            RowLayout{
                                Button{
                                    text:qsTr("校正CartCalibration")
                                    onClicked: {
                                        //logicManager.performCalibration("aa1_chart_calibration")
                                        logicManagerState.setCurrentCalibrationName("aa1_chart_calibration")
                                        logicManager.performHandling(LogicManager.PERFORM_CALIBRATION)
                                    }
                                }
                                Button{
                                    text:qsTr("执行ChartCalibrationPR")
                                    onClicked: {
                                        //logicManager.performLocation("aa1_chart_location")
                                        logicManagerState.setCurrentLocationName("aa1_chart_location")
                                        logicManagerState.setUseOriginPr(true)
                                        logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                                    }
                                }
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("计算")
                                    onClicked: {

                                    }
                                }
                                Button{
                                    text:qsTr("记录")
                                    onClicked: {

                                    }
                                }
                            }
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process8_operate.visible =false;
                                    btn_process8.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("9.确定AAmushroom位置的XYC坐标")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process9
                        onClicked: {
                            process9_operate.visible =true;
                            btn_process9.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process9_operate
                        ColumnLayout{
                            GroupBox{
                                title: qsTr("Mushroom Position")
                                AAHeadMushroomPosition {}
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("SUT退到DownLookPR位置")
                                    onClicked: {
                                        sutModule.moveToDownlookPR()
                                    }
                                }
                                Button{
                                    text:qsTr("LUT校正")
                                    onClicked: {

                                    }
                                }
                                Button{
                                    text:qsTr("LUT执行PR")
                                    onClicked: {

                                    }
                                }
                            }
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process9_operate.visible =false;
                                    btn_process9.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("10.确定LUT和LPA的位置关系")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process10
                        onClicked: {
                            process10_operate.visible =true;
                            btn_process10.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process10_operate
                        ColumnLayout{
                            GroupBox{
                                title: qsTr("Load Uplook Position")
                                LUTLoadUplookPosition {
                                }
                            }
                            GroupBox{
                                title:"download camera位置"
                                ColumnLayout{
                                    RowLayout{
                                        Label{
                                            text:qsTr("X")
                                        }
                                        TextField{
                                            text:lut_camera_position.X
                                            horizontalAlignment: TextInput.AlignHCenter
                                            validator: DoubleValidator{
                                                decimals: 6
                                                notation: DoubleValidator.StandardNotation
                                            }
                                            onEditingFinished: {
                                                lut_camera_position.setX(text)
                                            }
                                        }
                                        Label{
                                            text:qsTr("Y")
                                        }
                                        TextField{
                                            text:lut_camera_position.Y
                                            horizontalAlignment: TextInput.AlignHCenter
                                            validator: DoubleValidator{
                                                decimals: 6
                                                notation: DoubleValidator.StandardNotation
                                            }
                                            onEditingFinished: {
                                                lut_camera_position.setY(text)
                                            }
                                        }
                                    }
                                    RowLayout{
                                        Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                                        Button{
                                            text:title_move_to
                                            onClicked: {
                                                //logicManager.lensPickArmMoveToUpdownlookDownPos()
//                                                lensLoaderModule.performHandling(LensLoaderModule.UPDOWNLOOK_DOWN_POS)
                                                logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.UPDOWNLOOK_DOWN_POS)
                                            }
                                        }
                                        Button{
                                            text:title_read_encoder
                                            onClicked: {
                                                var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)-baseModuleManager.getPROffsetX(lensPickArmModuleParameter.lpaUpdownlookDownLocationName)
                                                var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)-baseModuleManager.getPROffsetY(lensPickArmModuleParameter.lpaUpdownlookDownLocationName)
                                                lut_camera_position.setX(x)
                                                lut_camera_position.setY(y)

                                            }
                                        }
                                    }
                                }
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("校正lpa_updownlook_up_calibration")
                                    onClicked: {
                                        //logicManager.performCalibration("lpa_updownlook_up_calibration")
                                        logicManagerState.setCurrentCalibrationName("lpa_updownlook_up_calibration")
                                        logicManager.performHandling(LogicManager.PERFORM_CALIBRATION)
                                    }
                                }
                                Button{
                                    text:qsTr("校正lpa_updownlook_down_calibration")
                                    onClicked: {
                                        //logicManager.performCalibration("lpa_updownlook_down_calibration")
                                        logicManagerState.setCurrentCalibrationName("lpa_updownlook_down_calibration")
                                        logicManager.performHandling(LogicManager.PERFORM_CALIBRATION)
                                    }
                                }
                            }
                            Button{
                                text:qsTr("执行lpa_updownlook_pr")
                                onClicked: {
                                    //logicManager.performLocation("lpa_updownlook_up_location")
                                    //logicManager.performLocation("lpa_updownlook_down_location")
                                    logicManagerState.setCurrentLocationName("lpa_updownlook_up_location")
                                    logicManagerState.setUseOriginPr(true)
                                    logicManager.performHandling(LogicManager.PERFORM_LOCATION)

                                    logicManagerState.setCurrentLocationName("lpa_updownlook_down_location")
                                    logicManagerState.setUseOriginPr(true)
                                    logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                                }
                            }

                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process10_operate.visible =false;
                                    btn_process10.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("11.确定lut_uplook_picker_position，放料角度")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process11
                        onClicked: {
                            process11_operate.visible =true;
                            btn_process11.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process11_operate
                        ColumnLayout{
                            GroupBox{
                                title:"upload picker位置"
                                ColumnLayout{
                                    RowLayout{
                                        Label{
                                            text:qsTr("X")
                                        }
                                        TextField{
                                            text:lut_picker_position.X
                                            horizontalAlignment: TextInput.AlignHCenter
                                            validator: DoubleValidator{
                                                decimals: 6
                                                notation: DoubleValidator.StandardNotation
                                            }
                                            onEditingFinished: {
                                                lut_picker_position.setX(text)
                                            }
                                        }
                                        Label{
                                            text:qsTr("Y")
                                        }
                                        TextField{
                                            text:lut_picker_position.Y
                                            horizontalAlignment: TextInput.AlignHCenter
                                            validator: DoubleValidator{
                                                decimals: 6
                                                notation: DoubleValidator.StandardNotation
                                            }
                                            onEditingFinished: {
                                                lut_picker_position.setY(text)
                                            }
                                        }
                                    }
                                    RowLayout{
                                        Layout.alignment: Qt.AlignVCenter|Qt.AlignRight
                                        Button{
                                            text:title_move_to
                                            onClicked: {
                                                //logicManager.lensPickArmMoveToUpdownlookUpPos()
//                                                lensLoaderModule.performHandling(LensLoaderModule.UPDOWNLOOK_UP_POS)
                                                logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.UPDOWNLOOK_UP_POS)
                                            }
                                        }
                                        Button{
                                            text:title_read_encoder
                                            onClicked: {
                                                var x = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorXName)
                                                var y = baseModuleManager.getMotorFeedbackPos(lensPickArmParams.motorYName)
                                                lut_picker_position.setX(x)
                                                lut_picker_position.setY(y)
                                            }
                                        }
                                    }
                                }
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("校正lut_downlook_calibration")
                                    onClicked: {

                                    }
                                }
                                Button{
                                    text:qsTr("执行lut_downlook_pr")
                                    onClicked: {

                                    }
                                }
                            }
                            Button{
                                text:qsTr("计算吸头偏移")
                                onClicked: {

                                }
                            }

                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process11_operate.visible =false;
                                    btn_process11.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("12.确定lpa_downlook_load_position和lpa_downlook_unload_position位置")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process12
                        onClicked: {
                            process12_operate.visible =true;
                            btn_process12.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process12_operate
                        ColumnLayout{
                            GroupBox{
                                title: qsTr("Load Position")
                                LUTLoadPositionView {
                                }
                            }

                            GroupBox{
                                title: qsTr("Unload Position")
                                LUTUnloadPosition {
                                }
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("校正")
                                    onClicked: {

                                    }
                                }
                                Button{
                                    text:qsTr("执行PR")
                                    onClicked: {

                                    }
                                }
                                Button{
                                    text:qsTr("计算")
                                    onClicked: {

                                    }
                                }
                            }
                            RowLayout{
                                Button{
                                    text:qsTr("取AANgLens");
                                    onClicked: {

                                    }
                                }
                                Button{
                                    text:qsTr("lpa取NGLens");
                                    onClicked: {
                                        //logicManager.lensPickArmMoveToPickLensFromLut()
//                                        lensLoaderModule.performHandling(LensLoaderModule.LUT_POS2+
//                                                                         LensLoaderModule.LUT_LENS_PR+
//                                                                         LensLoaderModule.ToWork+
//                                                                         LensLoaderModule.PICK_NG_LENS_FROM_LUT2)
                                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS2+
                                                                     LensLoaderModule.LUT_LENS_PR+
                                                                     LensLoaderModule.ToWork+
                                                                     LensLoaderModule.PICK_NG_LENS_FROM_LUT2)
                                    }
                                }
                                Button{
                                    text:qsTr("lpa放lens")
                                    onClicked: {
                                        //logicManager.lensPickArmMoveToPlaceLensToLut()
//                                        lensLoaderModule.performHandling(LensLoaderModule.LUT_POS1+
//                                                                         LensLoaderModule.RESET_PR+
//                                                                         LensLoaderModule.ToWork+
//                                                                         LensLoaderModule.PLACE_LENS_TO_LUT)
                                        logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LUT_POS1+
                                                                     LensLoaderModule.RESET_PR+
                                                                     LensLoaderModule.ToWork+
                                                                     LensLoaderModule.PLACE_LENS_TO_LUT)
                                    }
                                }
                            }

                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process12_operate.visible =false;
                                    btn_process12.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("13.LensTrayloader位置和操作")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process13
                        onClicked: {
                            process13_operate.visible =true;
                            btn_process13.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process13_operate
                        ColumnLayout{
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process13_operate.visible =false;
                                    btn_process13.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("14.Lens盘规格和操作")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process14
                        onClicked: {
                            process14_operate.visible =true;
                            btn_process14.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process14_operate
                        ColumnLayout{
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process14_operate.visible =false;
                                    btn_process14.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("15.SensorTrayloader位置和操作")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process15
                        onClicked: {
                            process15_operate.visible =true;
                            btn_process15.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process15_operate
                        ColumnLayout{
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process15_operate.visible =false;
                                    btn_process15.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("16.Sensor盘规格和操作")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process16
                        onClicked: {
                            process16_operate.visible =true;
                            btn_process16.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process16_operate
                        ColumnLayout{
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process16_operate.visible =false;
                                    btn_process16.visible = true;
                                }
                            }
                        }
                    }

                    Label{
                        text:qsTr("17.LPA的SUT位置和操作")
                    }
                    Button{
                        text:qsTr("显示操作细节")
                        id:btn_process17
                        onClicked: {
                            process17_operate.visible =true;
                            btn_process17.visible = false;
                        }
                    }
                    GroupBox{
                        title:"具体操作"
                        visible: false
                        id:process17_operate
                        ColumnLayout{
                            Button{
                                text:qsTr("隐藏操作细节")
                                onClicked: {
                                    process17_operate.visible =false;
                                    btn_process17.visible = true;
                                }
                            }
                        }
                    }

                }
            }

//            TreeView{
//                //backgroundVisible: false
//                implicitWidth:500
//                implicitHeight: 700
//                TableViewColumn{
//                    width:150*1.5
//                    title:"Title"
//                    role:"title"
//                }
//                TableViewColumn{
//                    width: 300*1.5
//                    title:"Operate"
//                    role:"operate"
//                }

//                model:checkProcessModel
//                itemDelegate: RowLayout{
//                    Text{
//                        wrapMode: Text.Wrap
//                        color:"black"
//                        text:styleData.value
//                        clip:true
//                    }
//                }
//            }
        }
    }
}
