#ifndef CONFIG_H
#define CONFIG_H
#include <QString>

#define CONFIG_DIR                   "config//"
#define PR_CONFIG_DIR                "config//prConfig"
#define SYSTERM_PARAM_DIR            "config//systermParameter//"
#define SYSTERM_CONGIF_FILE           "systermConfig.json"
#define SYSTERM_PARAM_FILE           "systermParameter.json"
#define SYSTERM_PARAMETER            "systermParameter"
#define MACHINE_PARAM_FILE           "xt_motion_config.csv"
#define SH_SENSOR_TRAY_FILE          "sensorTray.json"
#define SH_REJECT_TRAY_FILE          "rejectTray.json"
#define SH_LENS_TRAY_FILE            "lensTray.json"
#define AA_HEAD_FILE                 "aaHeadConfig.json"
#define DOTHINGKEY_FILE              "dothinkeyConfig.json"
#define DISPENSER_FILE               "dispenser_parameter.json"
#define DISPENSER_PARAMETER          "dispenser_parameter"
#define DISPENSE_MODULE_FILE         "dispense_module_parameter.json"
#define DISPENSER_MODULE_PARAMETER   "dispense_module_parameter"
#define VCM_PARAMETER_FILE          "vcm_parameter.json"
#define MOTOR_PARAMETER_FILE       "motor_parameter.json"
#define CYLINDER_PARAMETER_FILE     "cylinder_parameter.json"
#define VACUUM_PARAMETER_FILE       "vacuum_parameter.json"
#define VISION_LOCATION_PARAMETER_FILE   "vision_location_parameter.json"
#define CALIBRATION_PARAMETER_FILE   "calibration_parameter.json"
#define AA_CORE_MODULE_FILE          "aaCoreConfig.json"
#define MATERIAL_PICKARM_PARAMETER   "material_pickarm"
#define MATERIAL_PICKARM_FILE        "materialPickArmConfig.json"
#define MATERIAL_LOADER_FILE         "materialLoaderModule.json"
#define SH_LSUT_FILE                 "lsut_module.json"

#define BASE_LOG_DIR                 "log//"
#define SYSTEM_LOG_DIR               "system_log//"
#define GRABBER_DEBUG_DIR            "grabber_log//"
#define MTF_DEBUG_DIR                "mtf_log//"
#define UNIT_LOG_DIR                 "unit_log//"
#define VISION_PR_DEBUG_DIR          "pr_log//"
#define PRODUCE_PROCESS_DIR          "produce_process//"
#define DISPENSE_DIR              "dispense_log//"
#define BASE_MODULE_JSON             "config//baseModuleConfig.json"
#define AA_DEBUG_DIR                 "aa_log//"

#define PREVIEW_1                   "preview1"
#define PREVIEW_2                   "preview2"
#define PREVIEW_3                   "preview3"
#define PREVIEW_4                   "preview4"
#define PREVIEW_5                   "preview5"
#define PREVIEW_6                   "preview6"
#define PREVIEW_7                   "preview7"

//ToDo: Move to config file for the camera
#define CAMERA_SH_UT_UL                  "UT_ULCamera"
#define CAMERA_SH_PA_DL                  "PA_DLCamera"
#define CAMERA_SH_AA_DL                  "AA_DLCamera"

#define LIGHTING_PA_DL                  0
#define LIGHTING_AA_DL                  3
#define LIGHTING_SUT_UL                 2

#define DOWNLOOK_VISION_CAMERA_WIDTH    659
#define DOWNLOOK_VISION_CAMERA_HEIGHT   494

#define CONSTANT_REFERENCE 2304

//PR Learn
#define DISPENSE_PATH_2D_POINT_FILE     "config//vision_dispense_path//resultPath.avdata"

//calibration
#define CALIBRATION_RESULT_PATH          ".//config//calibrationConfig//"
#define CHART_CALIBRATION                "chart_calibration"

//dispenser
#define DISPENSER_PARAMETER_PATH        ".//config//dispenser_parameter.json"
#define DISPENSE_MODULE_PARAMETER_PATH  ".//config//dispense_module_parameter.json"

//Icon name
#define ICON_SPARROW         ":/icons/sparrow.png"

#define POS_ERROR           0.01

//Flowchart Params
#define AA_PIECE_START "Start"
#define AA_PIECE_LOAD_CAMERA "Load Camera"
#define AA_PIECE_INIT_CAMERA "Init Camera"
#define AA_PIECE_PR_TO_BOND "PR To Bond"
#define AA_PIECE_INITIAL_TILT "Initial Tilt"
#define AA_PIECE_LOAD_MATERIAL "Load Material"
#define AA_PIECE_UNLOAD_LENS "AA Unload Lens"
#define AA_PIECE_Z_OFFSET "Z Offset"
#define AA_PIECE_XY_OFFSET "XY Offset"
#define AA_PIECE_AA "AA"
#define AA_PIECE_OC "OC"
#define AA_PIECE_MTF "MTF"
#define AA_PIECE_UV "UV"
#define AA_PIECE_DISPENSE "Dispense"
#define AA_PIECE_Y_LEVEL "Y_Level"
#define AA_PIECE_DELAY "Delay"
#define AA_PIECE_JOIN "Join"
#define AA_PIECE_SAVE_IMAGE "Save Image"
#define AA_PIECE_ACCEPT "Accept"
#define AA_PIECE_REJECT "Reject"
#define AA_PIECE_TERMINATE "Terminate"
#define AA_PIECE_GRR "GRR"

#define AA_UNLOAD_CAMERA "Unload Camera"


//AA Params ToDo Move to System Property
#define AA_MAX_INTENSITY 50
#define AA_MIN_AREA 10000
#define AA_MAX_AREA 90000

#endif
// CONFIG_H
