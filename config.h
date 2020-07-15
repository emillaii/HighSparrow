#ifndef CONFIG_H
#define CONFIG_H
#include <QString>

#define CONFIG_DIR                   "config//"
#define STATES_DIR                   "states//"
#define PR_CONFIG_DIR                "config//prConfig"
#define SYSTERM_PARAM_DIR            "config//systermParameter//"
#define Y_LEVEL_PATH_DIR             "config//y_level_path//"
#define SYSTERM_CONGIF_FILE          "systermConfig.json"
#define SYSTERM_PARAM_FILE           "systermParameter.json"
#define SYSTERM_PARAMETER            "systermParameter"
#define MACHINE_PARAM_FILE           "xt_motion_config.csv"
#define TCP_CONFIG_FILE              "tcpConfig.json"
#define MATERIAL_TRAY_FILE           "materilTray.json"
#define AA_HEAD_FILE                 "aaHeadConfig.json"
#define SUT_FILE                     "sutConfig.json"
#define DOTHINGKEY_FILE              "dothinkeyConfig.json"
#define DISPENSER_FILE               "dispenser_parameter.json"
#define DISPENSER_PARAMETER          "dispenser_parameter"
#define DISPENSE_MODULE_FILE         "dispense_module_parameter.json"
#define DISPENSER_MODULE_PARAMETER   "dispense_module_parameter"
#define TRAY_LOADER_FILE             "tray_loader_parameter.json"
#define TRAY_LOADER_PARAMETER        "tray_loader_parameter"
//trayclip
#define TRAY_CLIPOUT_FILE            "tray_clipout_parameter.json"
#define TRAY_CLIPIN_FILE            "tray_clipin_parameter.json"
#define TRAY_CLIPIN_PARAMETER       "tray_clipin_parameter"
#define TRAY_CLIPOUT_PARAMETER      "tray_clipout_parameter"
#define SENSOR_LOADER_FILE          "SensorLoaderModule.json"
#define SENSOR_PICKARM_FILE         "sensorPickArmConfig.json"
#define SENSOR_PICKARM_PARAMETER     "sensor_pickarm"
#define SENSOR_TRAY_LOADER_FILE     "sensorTrayLoaderConfig.json"
#define LUT_CARRIER_FILE            "lutCarrierConfig.json"
#define LUT_CARRIER_PARAMETER       "lut"
#define LUT_FILE                    "lutConfig.json"
#define LENS_LOADER_MODULE_FILE     "lensLoaderModule.json"
#define LENS_PICKARM_FILE           "lensPickArmConfig.json"
#define LENS_PICKARM_PARAMETER       "lens_pickarm"
#define SUT_CARRIER_FILE             "sutCarrierConfig.json"
#define VCM_PARAMETER_FILE          "vcm_parameter.json"
#define MOTOR_PARAMETER_FILE       "motor_parameter.json"
#define CYLINDER_PARAMETER_FILE     "cylinder_parameter.json"
#define VACUUM_PARAMETER_FILE       "vacuum_parameter.json"
#define VISION_LOCATION_PARAMETER_FILE   "vision_location_parameter.json"
#define LIMIT_PARAMETER_FILE        "limit_parameter.json"
#define CALIBRATION_PARAMETER_FILE   "calibration_parameter.json"
#define AA_CORE_MODULE_FILE          "aaCoreConfig.json"


#define VISION_CONFIG_DIR            "config//vision_config_1"
#define AA_HELPER_DIR                "aa_helper_log//"
#define BASE_LOG_DIR                 "log//"
#define SYSTEM_LOG_DIR               "system_log//"
#define GRABBER_DEBUG_DIR            "grabber_log//"
#define MTF_DEBUG_DIR                "mtf_log//"
#define UNIT_LOG_DIR                 "unit_log//"
#define ALARM_LOG_DIR                "alarm_log//"
#define REJECT_LOG_DIR               "reject_log//"
#define PARTICAL_CHECK_DIR           "partical_check_log//"
#define AA_LOG_DIR                   "aa_log//"
#define PERFORMANCE_DIR              "performance//"
#define AA_PROCESS_DIR               "aa_process//"
#define PRODUCE_PROCESS_DIR          "produce_process//"
#define VISION_PR_DEBUG_DIR          "pr_log//"
#define DISPENSE_VISION_PR_DIR       "dispense_log//"
#define GLUE_INPSECTION_DIR          "glue_inspection//"
#define Y_LEVEL_DIR                  "y_level_log//"
#define AA_DEBUG_DIR                 "aa_log//"
#define AA_SAVE_IMAGE_DIR            "aa_save_image//"
#define AA_SAVE_IMAGE_DOWNLOOK_DIR   "aa_save_image//downlook//"
#define AA_SAVE_IMAGE_UPLOOK_DIR     "aa_save_image//uplook//"
#define AA_SAVE_IMAGE_PICKARM_DIR    "aa_save_image//pickarm//"
#define FLOWCHART_CONFIG_JSON        "config//flowchart//flowchart.json"
#define POSITION_CONFIG_JSON         "config//positionConfig.json"
#define SYSTEM_CONFIG_JSON           "config//systemConfig.json"
#define MATERIAL_LOADING_CONFIG_JSON "config//materialLoadingConfig.json"
#define AA_CALIBRATION_CONFIG_JSON   "config//aaCalibrationConfig.json"
#define PR_CONFIG_JSON               "config//prConfig.json"
#define AA_HEAD_MODULE_JSON          "config//aaHeadConfig.json"
#define AA_CORE_MODULE_JSON          "config//aaCoreConfig.json"
#define SUT_MODULE_JSON              "config//sutConfig.json"
#define LUT_MODULE_JSON              "config//lutConfig.json"
#define BASE_MODULE_JSON             "config//baseModuleConfig.json"
#define LUT_CARRIER_FILE_NAME        "config//lutCarrierConfig.json"
#define LENS_PICKARM_FILE_NAME       "config//lensPickArmConfig.json"
#define SENSOR_PICKARM_FILE_NAME     "config//sensorPickArmConfig.json"
#define SENSOR_TRAY_LOADER_FILE_NAME "config//sensorTrayLoaderConfig.json"
#define SUT_CARRIER_FILE_NAME        "config//sutCarrierConfig.json"
#define VCM_PARAMETER_FILENAME       "config//vcm_parameter.json"
#define MOTOR_PARAMETER_FILENAME     "config//motor_parameter.json"
#define CYLINDER_PARAMETER_FILENAME  "config//cylinder_parameter.json"
#define VACUUM_PARAMETER_FILENAME    "config//vacuum_parameter.json"
#define VISION_LOCATION_PARAMETER_FILENAME   "config//vision_location_parameter.json"
#define LIMIT_PARAMETER_FILENAME   "config//limit_parameter.json"
#define LIMIT_PARAMETER_MODE_FILENAME   "demo//limit_parameter.json"
#define CALIBRATION_PARAMETER_FILENAME   "config//calibration_parameter.json"
#define VISION_DISPENSE_PATH         "config//vision_dispense_path//resultImageWithPath.jpg"
#define FLOWCHART_URL                "http://127.0.0.1:8000/flowchart.html"
#define SHARE_DIR                   "\\\\AA1\\r\\temp\\"


//Title naming
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

#define CAMERA_AA1_DL                    "AA_DLCamera1"
#define CAMERA_AA2_DL                    "AA_DLCamera2"
#define CAMERA_LPA_DL                    "LPA_DLCamera"
#define CAMERA_LUT_DL                    "LUT_ULCamera"
#define CAMERA_SPA_DL                    "SPA_DLCamera"
#define CAMERA_LPA_UL                    "Sensor_ULCamera"
#define CAMERA_LPA_BARCODE               "Barcode_Camera"

#define LIGHTING_LUT_UL                 0
#define LIGHTING_AA1_DL_COAXIAL         1
#define LIGHTING_AA1_DL                 2
#define LIGHTING_LPA_DL                 3
#define LIGHTING_SENSOR_UL              1	// not used for now

#define LIGHTING_SPA_DL                 0
#define LIGHTING_AA2_DL                 1
#define LIGHTING_AA2_DL_COAXIAL         2

#define CURRENT_PR_RESULT               "CurrentPRResult"

#define DOWNLOOK_VISION_CAMERA_WIDTH    659
#define DOWNLOOK_VISION_CAMERA_HEIGHT   494

#define UPLOOK_VISION_CAMERA_WIDTH      659
#define UPLOOK_VISION_CAMERA_HEIGHT     494

#define CONSTANT_REFERENCE 2448.0

#define DISPENSE_DIR                    "config//vision_dispense_path"
#define DISPENSE_PATH_EXE               "DrawPath.avexe"
#define DISPENSE_PATH_IMAGE_LOCATION    "config//vision_dispense_path//resultImageWithPath.jpg"
#define DISPENSE_PATH_CIRCLE_PROPERTIES "config//vision_dispense_path/resultCircle.json"
#define DISPENSE_PATH_2D_POINT_FILE     "config//vision_dispense_path//resultPath.avdata"

#define UPLOOK_LENS_HOLDER_IMX363_EDGE_MODEL      "config//vision_config_1//Lens.4e95ee99.avdata"
#define DOWNLOOK_CMOS_IMX363_EDGE_MODEL           "config//vision_config_1//downlook_sensor.ad9e6e81.avdata"
//calibration
#define CALIBRATION_RESULT_PATH          ".//config//calibrationConfig//"

#define CHART_CALIBRATION                "chart_calibration"
//dispenser

#define DISPENSER_PARAMETER_PATH        ".//config//dispenser_parameter.json"
#define DISPENSE_MODULE_PARAMETER_PATH  ".//config//dispense_module_parameter.json"

//trayloader
#define TRAY_LOADER_PATH  ".//config//tray_loader_parameter.json"

//trayclip
#define TRAY_CLIPOUT_PATH  ".//config//tray_clipout_parameter.json"
#define TRAY_CLIPIN_PATH  ".//config//tray_clipin_parameter.json"
//PR

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
#define PR_LENS_LPALOOK                  "pr_lens_lpalook"
#define PR_VACANCY_LPALOOK               "pr_vacancy_lpalook"
#define PR_LENS_LUTLOOK                  "pr_lens_lutlook"

//Lighting Config
#define LIGHTING_UPLOOK_LENS_HOLDER_PR         "uplookLensHolderPRLighting"
#define LIGHTING_DOWNLOOK_CMOS_PR              "downlookCmosPRLighting"
#define LIGHTING_UPDOWNLOOK_CALIB_GLASS_PR     "updownlookCalibGlassPRLighting"
#define LIGHTING_PICKARM_LENS_HOLDER_PR        "pickarmLensHolderPRLighting"
#define LIGHTING_PICKARM_CMOS_PR               "pickarmCmosPRLighting"
#define LIGHTING_PICKARM_PRODUCT_PR            "pickarmProductPRLighting"

//Icon name
#define ICON_SPARROW         ":/icons/sparrow.png"

//Flowchart Params
#define AA_PIECE_START "Start"
#define AA_PIECE_LOAD_CAMERA "Load Camera"
#define AA_PIECE_INIT_LENS "Init_Lens"
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
#define AA_PIECE_OTP "OTP"
//#define AA_PIECE_SAVE_IMAGE "Save Image"
#define AA_PIECE_DELAY "Delay"
#define AA_PIECE_JOIN "Join"
#define AA_PIECE_SAVE_IMAGE "Save Image"
#define AA_PIECE_ACCEPT "Accept"
#define AA_PIECE_REJECT "Reject"
#define AA_PIECE_TERMINATE "Terminate"
#define AA_PIECE_GRR "GRR"
#define AA_PIECE_PARTICAL_CHECK "Partical Check"

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

#define UNIT_LOG_PORT 5044
#define SFR_LOG_PORT 5045

#define VISION_MODULE_1 "VisionModule_1"
#define VISION_MODULE_2 "VisionModule_2"
#define LIGHTING_CONTROLLER_1 "LightController_1"
#define LIGHTING_CONTROLLER_2 "LightController_2"

#endif
// CONFIG_H
