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
#define LIMIT_PARAMETER_FILE        "limit_parameter.json"
#define CALIBRATION_PARAMETER_FILE   "calibration_parameter.json"
#define AA_CORE_MODULE_FILE          "aaCoreConfig.json"

#define MATERIAL_PICKARM_PARAMETER   "material_pickarm"
#define MATERIAL_PICKARM_FILE        "materialPickArmConfig.json"
#define MATERIAL_LOADER_PARAMETER    "material_loader"
#define MATERIAL_LOADER_FILE         "materialLoaderModule.json"
#define SH_LSUT_FILE                 "lsut_module.json"

#define BASE_LOG_DIR                 "log//"
#define SYSTEM_LOG_DIR               "system_log//"
#define GRABBER_DEBUG_DIR            "grabber_log//"
#define MTF_DEBUG_DIR                "mtf_log//"
#define VISION_PR_DEBUG_DIR          "pr_log//"
#define AA_DEBUG_DIR                 "aa_log//"
#define DISPENSE_DIR              "dispense_log//"
#define SYSTEM_CONFIG_JSON           "config//systemConfig.json"
#define BASE_MODULE_JSON             "config//baseModuleConfig.json"
#define LIMIT_PARAMETER_MODE_FILENAME   "demo//limit_parameter.json"

#define MOTION_DIALOG_TITLE         u8"馬達"
#define POSITION_LEARNING_TITLE     u8"位置紀錄"
#define FLOWCHART_TITLE             u8"流程图"

#define PREVIEW_1                   "preview1"
#define PREVIEW_2                   "preview2"
#define PREVIEW_3                   "preview3"
#define PREVIEW_4                   "preview4"
#define PREVIEW_5                   "preview5"
#define PREVIEW_6                   "preview6"
#define PREVIEW_7                   "preview7"

//ToDo: Move to config file for the camera
#define DOWNLOOK_VISION_CAMERA          "AA_DLCamera1"
#define UPLOOK_VISION_CAMERA            "LUT_ULCamera"
#define PICKARM_VISION_CAMERA           "LPA_DLCamera"

#define CAMERA_SH_UT_UL                  "UT_ULCamera"
#define CAMERA_SH_PA_DL                  "PA_DLCamera"
#define CAMERA_SH_AA_DL                  "AA_DLCamera"

#define LIGHTING_PA_DL                  0
#define LIGHTING_AA_DL                  3
#define LIGHTING_SUT_UL                 2

#define CURRENT_PR_RESULT               "CurrentPRResult"

#define DOWNLOOK_VISION_CAMERA_WIDTH    659
#define DOWNLOOK_VISION_CAMERA_HEIGHT   494

#define UPLOOK_VISION_CAMERA_WIDTH      659
#define UPLOOK_VISION_CAMERA_HEIGHT     494

#define CONSTANT_REFERENCE 2304

//PR Execuatable for HMI
#define SAVE_IMAGE_FROM_BASLER_EXEC     "SaveImage.avexe"
#define LEARN_PR_DOWNLOOK_SENSOR_EXEC   "LearnPR_Downlook_Sensor.avexe"
#define LEARN_PR_UPLOOK_MUSHROOM_EXEC   "LearnPR_Uplook_Mushroom.avexe"

//PR Learn
#define DISPENSE_PATH_2D_POINT_FILE     "config//vision_dispense_path//resultPath.avdata"

//calibration
#define CALIBRATION_RESULT_PATH          ".//config//calibrationConfig//"
#define CHART_CALIBRATION                "chart_calibration"

//dispenser
#define DISPENSER_PARAMETER_PATH        ".//config//dispenser_parameter.json"
#define DISPENSE_MODULE_PARAMETER_PATH  ".//config//dispense_module_parameter.json"

//PR
#define PR_MUSHROOM_PR                  "mushroom_pr"
#define PR_DOWNLOOK_CMOS_PR             "downlook_cmos_pr"
#define PR_UPLOOK_LENS_HOLDER_PR        "uplook_lens_holder_pr"
#define PR_PICKARM_CMOS_PR              "pickarm_cmos_pr"
#define PR_PICKARM_LENS_HOLDER_PR       "pickarm_lens_holder_pr"
#define PR_UPDOWNLOOK_CALIB_GLASS_PR    "updownlook_calib_glass_pr"
#define PR_PICKARM_LENS_TREAY_PR        "pickarm_lens_tray_pr"

#define PR_PARAMETER_FILE_PATH           ".//config//pr_param//"
#define PR_AA1_MUSHROOMHEAD              "pr_aa1_mushroom"
#define PR_AA1_TOOL_UPLOOK               "pr_aa1_tool_uplook"
#define PR_AA1_TOOL_DOWNLOOK             "pr_aa1_tool_downlook"
#define PR_AA1_LUT_UPLOOK                "pr_aa1_lut_uplook"
#define PR_AA2_TOOL_UPLOOK               "pr_aa2_tool_uplook"
#define PR_AA2_TOOL_DOWNLOOK             "pr_aa2_tool_downlook"
#define PR_AA2_LUT_UPLOOK                "pr_aa2_lut_uplook"
#define PR_SUT_DOWNLOOK                  "pr_sut_downlook"
#define PR_LOAD_LUT_UPLOOK               "pr_load_lut_uplook"
#define PR_LENS_LPALOOK               "pr_lens_lpalook"
#define PR_VACANCY_LPALOOK               "pr_vacancy_lpalook"
#define PR_LENS_LUTLOOK               "pr_lens_lutlook"

//AA Calibration Config
#define AA_CALIBRATION       "aaCalibration"
#define AA_CALIBRATION_A11   "a11"
#define AA_CALIBRATION_A12   "a12"
#define AA_CALIBRATION_A13   "a13"
#define AA_CALIBRATION_A21   "a21"
#define AA_CALIBRATION_A22   "a22"
#define AA_CALIBRATION_A23   "a23"
#define AA_CALIBRATION_THETA "theta"

//Icon name
#define ICON_SPARROW         ":/icons/sparrow.png"

#define POS_ERROR           0.01

//Flowchart Params
#define AA_PIECE_START "Start"
#define AA_PIECE_LOAD_CAMERA "Load Camera"
#define AA_PIECE_INIT_CAMERA "Init Camera"
#define AA_PIECE_PR_TO_BOND "PR To Bond"
#define AA_PIECE_INITIAL_TILT "Initial Tilt"
#define AA_PIECE_PICK_LENS "AA Pick Lens"
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
//#define AA_PIECE_SAVE_IMAGE "Save Image"
#define AA_PIECE_DELAY "Delay"
#define AA_PIECE_JOIN "Join"
#define AA_PIECE_SAVE_IMAGE "Save Image"
#define AA_PIECE_ACCEPT "Accept"
#define AA_PIECE_REJECT "Reject"
#define AA_PIECE_TERMINATE "Terminate"
#define AA_PIECE_GRR "GRR"

#define AA_PIECE_INITIAL_POSITION "Init Pos"
#define AA_PIECE_INIT_SENSOR "Init Sensor"
#define AA_LOAD_CAMERA "Load Camera"
#define AA_PR_TO_BOND "PR2Bond"
#define AA_DISPENSE  "Disp"
#define AA_UV        "UV"
#define AA_UNLOAD_CAMERA "Unload Camera"
#define AA_DELAY     "DELAY"
#define AA_PIECE_END "END"

#define AA_PARAM_DIRECTION "direction"
#define AA_PARAM_MODE "mode"
#define AA_PARAM_STEP_SIZE "step_size"
#define AA_PARAM_START_Z "start_Z"
#define AA_PARAM_STOP_Z "final_Z"
#define AA_PARAM_ESTIMATED_FOV_SLOPE "estimated_fov_slope"
#define AA_PARAM_DELAY_Z "delay_Z_ms"
#define AA_PARAM_WAIT_TILT "wait_tilt"
#define AA_PARAM_TARGET_FOV "target_fov"
#define AA_PARAM_EDGE_FILTER "edge_filter"
#define AA_PARAM_IS_DEBUG "is_debug"
#define AA_LOOP_TIME "loop_time"
#define AA_PARAM_INIT_X_TILT "initial_x_tilt"
#define AA_PARAM_INIT_Y_TILT "initial_y_tilt"
#define AA_PARAM_X_TILT_OFFSET "x_tilt_offset"
#define AA_PARAM_Y_TILT_OFFSET "y_tilt_offset"
#define AA_PARAM_OC_FAST_MODE "fast_mode"
#define AA_PARAM_RETRY_COUNT "retry"
#define AA_PARAM_PR_CHECK "pr_check"
#define AA_PARAM_UV_EXPOSURE_TIME "time"
#define AA_PARAM_Z_OFFSET_TYPE "type"
#define AA_PARAM_Z_OFFSET_IN_UM "z_offset_in_um"
#define AA_PARAM_DELAY_IN_MS "delay"

//AA Params ToDo Move to System Property
#define AA_MAX_INTENSITY 50
#define AA_MIN_AREA 10000
#define AA_MAX_AREA 90000

#define UNIT_LOG_PORT 5044
#define SFR_LOG_PORT 5045

#endif
// CONFIG_H
