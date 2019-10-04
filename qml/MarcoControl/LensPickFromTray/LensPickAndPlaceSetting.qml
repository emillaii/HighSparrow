import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../../../"
import SomeLib 1.1
ColumnLayout{
    RowLayout { Label { text: qsTr("取放参数") } }
    RowLayout {
        Label {
            text:qsTr("力限制")
        }
        TextField{
            text:lensPickArmModuleParameter.vcmWorkForce
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lensPickArmModuleParameter.setVcmWorkForce(text)
            }
        }
        Label{
            text:qsTr("速度")
        }
        TextField{
            text:lensPickArmModuleParameter.vcmWorkSpeed
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lensPickArmModuleParameter.setVcmWorkSpeed(text)
            }
        }
        Label{
            text:qsTr("限力区间")
        }
        TextField{
            text:lensPickArmModuleParameter.vcmMargin
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lensPickArmModuleParameter.setVcmMargin(text)
            }
        }
    }
    RowLayout{
        Label{
            text:qsTr("Lens 物料高度")
        }
        TextField{
            text:lensPickArmModuleParameter.placeLensZ
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
                lensPickArmModuleParameter.setPlaceLensZ(text)
            }
        }
        Button{
            text:qsTr("lens视觉")
            width:40
            height:40
            onClicked: {
                if(baseModuleManager.getServerMode()===0){
                    logicManagerState.setCurrentLocationName(lens_loader_parameter.lensLocationName)
                    logicManagerState.setUseOriginPr(true)
                    logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.TO_PR_OFFSET
                                                 + SensorLoaderModule.TRAY_SENSOR_PR
                                                 + SensorLoaderModule.SENSOR_TRAY_1_POS)
                }
            }
        }
        Button{
            text:qsTr("移动吸头")
            width: 40
            height: 40
            onClicked: {
                if (baseModuleManager.getServerMode() === 0){
                    logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_PR+
                                                 LensLoaderModule.ToWork)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.TO_PICK_SENSOR_OFFSET
                                                 + SensorLoaderModule.TRAY_SENSOR_PR
                                                 + SensorLoaderModule.SENSOR_TRAY_1_POS)
                }
            }
        }
        Button{
            text:qsTr("测高")
            width: 40
            height: 40
            onClicked: {
                if (baseModuleManager.getServerMode() === 0){
                    logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.MeasureLensInTray)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.MEASURE_SENSOR_IN_TRAY1)
                }
            }
        }
    }
    RowLayout{
        Label { text:qsTr("Pick Lens Offset") }
        Label{
            text:qsTr("X")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
        Label{
            text:qsTr("Y")
        }
        TextField{
            text: "0"
            horizontalAlignment: TextInput.AlignHCenter
            validator: DoubleValidator{
                decimals: 6
                notation: DoubleValidator.StandardNotation
            }
            onEditingFinished: {
            }
        }
    }
    RowLayout {
        Label { text: qsTr("Place Lens Offset") }
        RowLayout{
            Label{
                text:qsTr("X")
            }
            TextField{
                text: "0"
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                }
            }
            Label{
                text:qsTr("Y")
            }
            TextField{
                text: "0"
                horizontalAlignment: TextInput.AlignHCenter
                validator: DoubleValidator{
                    decimals: 6
                    notation: DoubleValidator.StandardNotation
                }
                onEditingFinished: {
                }
            }
        }
    }
    RowLayout { Label { text: qsTr("测试") }}
    RowLayout {
        Label { text: qsTr("Tray1 目标位置") }
        Label { text: qsTr("目标行") }
        TextField{
            id:t_nrow
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
        }
        Label{
            text: qsTr("目标列")
        }
        TextField{
            id:t_ncol
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
        }
        Button{
            text:qsTr("下次取料位置")
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayFirst(t_ncol.text-1,t_nrow.text-1,0)
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                if (baseModuleManager.getServerMode() === 0){
                    logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_1_POS)
                }
            }
        }
    }
    RowLayout {
        Label { text: qsTr("Tray2 目标位置") }
        Label{
            text: qsTr("目标行")
        }
        TextField{
            id:t_nrow_2
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
        }
        Label{
            text: qsTr("目标列")
        }
        TextField{
            id:t_ncol_2
            text: "1"
            horizontalAlignment: TextInput.AlignHCenter
            validator: IntValidator{
                bottom: 1
            }
        }
        Button{
            text:qsTr("下次取料位置")
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayFirst(t_ncol_2.text-1,t_nrow_2.text-1,1)
            }
        }
        Button{
            text:title_move_to
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayCurrent(t_ncol_2.text-1,t_nrow_2.text-1,1)
                if (baseModuleManager.getServerMode() === 0){
                    logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY2)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.SENSOR_TRAY_2_POS)
                }
            }
        }
    }
    RowLayout {
        Button{
            text:qsTr("Lens 视觉")
            width:40
            height:40
            onClicked: {
                if(baseModuleManager.getServerMode()===0){
                    //logicManager.performLocation(lens_loader_parameter.lensLocationName);
                    logicManagerState.setCurrentLocationName(lens_loader_parameter.lensLocationName)
                    logicManagerState.setUseOriginPr(true)
                    logicManager.performHandling(LogicManager.PERFORM_LOCATION)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.TO_PR_OFFSET
                                                 + SensorLoaderModule.TRAY_SENSOR_PR
                                                 + SensorLoaderModule.SENSOR_TRAY_1_POS)
                }
            }
        }
        Button{
            text:qsTr("取lens")
            width: 40
            height: 40
            onClicked: {
                material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                if (baseModuleManager.getServerMode() === 0){
                    logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                                 LensLoaderModule.LENS_PR+
                                                 LensLoaderModule.ToWork+
                                                 LensLoaderModule.PICK_LENS_FROM_TRAY)
                }else{
                    logicManager.performHandling(sensorLoaderParameter.moduleName,SensorLoaderModule.PICK_SENSOR_FROM_TRAY
                                                 +SensorLoaderModule.TO_PICK_SENSOR_OFFSET
                                                 +SensorLoaderModule.TRAY_SENSOR_PR
                                                 +SensorLoaderModule.SENSOR_TRAY_1_POS)
                }
            }
        }
    }
    RowLayout {
        Button{
            text:qsTr("Lens Tray 空位视觉")
            onClicked: {
                logicManagerState.setCurrentLocationName(lens_loader_parameter.vacancyLocationName)
                logicManagerState.setUseOriginPr(true)
                logicManager.performHandling(LogicManager.PERFORM_LOCATION)
            }
        }
        Button{
            text:qsTr("放lens")
            onClicked: {
                material_tray.setTrayCurrent(t_ncol.text-1,t_nrow.text-1,0)
                logicManager.performHandling(lens_loader_parameter.moduleName,LensLoaderModule.LENS_TRAY1+
                                             LensLoaderModule.VACANCY_PR+
                                             LensLoaderModule.ToWork+
                                             LensLoaderModule.PLACE_NG_LENS_TO_TRAY)
            }
        }
    }
    RowLayout {
        Button{ text: qsTr("LPA VAC Open/Closed") }
        RoundButton{
            background: Rectangle {
                radius: 6
                color: "red"
            }
        }
    }
}
