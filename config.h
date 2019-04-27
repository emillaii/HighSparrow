#ifndef CONFIG_H
#define CONFIG_H
#include <QString>

#define CONFIG_DIR                   "config//"
#define PR_CONFIG_DIR                "config//prConfig"
#define VISION_CONFIG_DIR            "config//vision_config_1"
#define AA_HELPER_DIR                "aa_helper_log//"
#define BASE_LOG_DIR                 "log//"
#define SYSTEM_LOG_DIR               "system_log//"
#define GRABBER_DEBUG_DIR            "grabber_log//"
#define MTF_DEBUG_DIR                "mtf_log//"
#define VISION_PR_DEBUG_DIR          "pr_log//"
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
#define LENS_PICKARM_FILE_NAME        "config//lensPickArmConfig.json"
#define SUT_CARRIER_FILE_NAME        "config//sutCarrierConfig.json"
#define VCM_PARAMETER_FILENAME       "config//vcm_parameter.json"
#define CYLINDER_PARAMETER_FILENAME  "config//cylinder_parameter.json"
#define VACUUM_PARAMETER_FILENAME    "config//vacuum_parameter.json"
#define VISION_LOCATION_PARAMETER_FILENAME   "config//vision_location_parameter.json"
#define CALIBRATION_PARAMETER_FILENAME   "config//calibration_parameter.json"
#define FLOWCHART_URL                "http://127.0.0.1:8000/flowchart.html"

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

#define LIGHTING_LUT_UL                 0
#define LIGHTING_AA1_DL                 2
#define LIGHTING_LPA_DL                 3

#define LIGHTING_SPA_DL                 0
#define LIGHTING_AA2_DL                 1

#define CURRENT_PR_RESULT               "CurrentPRResult"

#define DOWNLOOK_VISION_CAMERA_WIDTH    659
#define DOWNLOOK_VISION_CAMERA_HEIGHT   494

#define UPLOOK_VISION_CAMERA_WIDTH      659
#define UPLOOK_VISION_CAMERA_HEIGHT     494

//PR Execuatable for HMI
#define SAVE_IMAGE_FROM_BASLER_EXEC     "SaveImage.avexe"
#define LEARN_PR_DOWNLOOK_SENSOR_EXEC   "LearnPR_Downlook_Sensor.avexe"
#define LEARN_PR_UPLOOK_MUSHROOM_EXEC   "LearnPR_Uplook_Mushroom.avexe"

//PR Learn
#define PR_DOWNLOOK_SENSOR_FILENAME     ".//config//vision_config_1//downlook_sensor.avdata"
#define PR_UPLOOK_MUSHROOM_FILENAME     ".//config//vision_config_1//uplook_mushroom.avdata"

#define PR_DOWNLOOK_CALI_GLASS_FILENAME   ".//config//vision_config_1//downlook_calib_glass.avdata"
#define PR_UPLOOK_CALI_GLASS_FILENAME     ".//config//vision_config_1//uplook_calib_glass.avdata"

#define UpDownlook_Calibration_Glass_Region_FILENAME ".//config//vision_config_1//UpDownlook_Calibration_Glass.226f8c29.Region.avdata"
#define UPLOOK_LENS_HOLDER_ADVANCE_REGION_FILENAME ".//config//vision_config_1//Lens.ee860be7.avdata"

#define PR_ROOT                         ".//vision_pr_system"
#define PR_M1                           "M1_V.avexe"
#define PR_M2                           "M2_V.avexe"
#define PR_M3                           "M3_V.avexe"
#define PR_M4                           "M4_V.avexe"
#define PR_M5                           "M5_V.avexe"

#define DISPENSE_DIR                    "config//vision_dispense_path"
#define DISPENSE_PATH_EXE               "DrawPath.avexe"
#define DISPENSE_PATH_IMAGE_LOCATION    "config//vision_dispense_path//resultImageWithPath.jpg"
#define DISPENSE_PATH_2D_POINT_FILE     "config//vision_dispense_path//resultPath.avdata"

#define UPLOOK_LENS_HOLDER_IMX363_EDGE_MODEL      "config//vision_config_1//Lens.4e95ee99.avdata"
#define DOWNLOOK_CMOS_IMX363_EDGE_MODEL           "config//vision_config_1//downlook_sensor.ad9e6e81.avdata"
//calibration
#define CALIBRATION_RESULT_PATH          ".//config//calibrationConfig//"
#define LENS_PICKARM_CALIBRATION         "lens_pickarm_calibration"
#define SENSOR_PICKARM_CALIBRATION       "sensor_pickarm_calibration"
#define AA1_UPLOOK_CALIBRATION           "aa1_upLook_calibration"
#define AA2_UPLOOK_CALIBRATION           "aa2_upLook_calibration"
#define AA1_DOWNLOOK_CALIBRATION         "aa1_downLook_calibration"
#define AA2_DOWNLOOK_CALIBRATION         "aa2_downLook_calibration"
#define AA1_UPDownLOOK_UP_CALIBRATION    "aa1_updownLook_up_calibration"
#define AA1_UPDownLOOK_DOWN_CALIBRATION  "aa1_updownLook_down_calibration"
#define AA2_UPDownLOOK_UP_CALIBRATION    "aa2_updownLook_up_calibration"
#define AA2_UPDownLOOK_DOWN_CALIBRATION  "aa2_updownLook_down_calibration"
#define AA1_MUSHROOMHEAD_CALIBRATION     "aa1_mushroomhead_calibration"
#define LPA_LENS_CALIBRATION               "lpa_lens_calibration"
#define SPA_SENSOR_CALIBRATION               "spa_sensor_calibration"


#define CHART_CALIBRATION                "chart_calibration"
//dispenser

#define DISPENSER_PARAMETER_PATH        ".//config//dispenser_parameter.json"
#define DISPENSER_PARAMETER        "dispenser_parameter"
#define DISPENSE_MODULE_PARAMETER_PATH  ".//config//dispense_module_parameter.json"
#define DISPENSER_MODULE_PARAMETER        "dispense_module_parameter"

//trayloader
#define TRAY_LOADER_PATH  ".//config//tray_loader_parameter.json"
#define TRAY_LOADER_PARAMETER "tray_loader_parameter"

//PR
#define PR_SENSOR_PACKAGE_IMX362        "IMX362"
#define PR_SENSOR_PACKAGE_IMX363        "IMX363"
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


//Lighting Config
#define LIGHTING_UPLOOK_LENS_HOLDER_PR         "uplookLensHolderPRLighting"
#define LIGHTING_DOWNLOOK_CMOS_PR              "downlookCmosPRLighting"
#define LIGHTING_UPDOWNLOOK_CALIB_GLASS_PR     "updownlookCalibGlassPRLighting"
#define LIGHTING_PICKARM_LENS_HOLDER_PR        "pickarmLensHolderPRLighting"
#define LIGHTING_PICKARM_CMOS_PR               "pickarmCmosPRLighting"
#define LIGHTING_PICKARM_PRODUCT_PR            "pickarmProductPRLighting"

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
#define ICON_SETTING         ":/images/icon/settings_input_composite.png"
#define ICON_POWER_ON        ":/images/icon/poweron.png"
#define ICON_POWER_OFF        ":/images/icon/poweroff.png"
#define ICON_STATUS_OFF      ":/images/icon/icon_status-dot-red"
#define ICON_STATUS_ON       ":/images/icon/icon_status-dot-green"
#define ICON_CAMERA_OFF      ":/images/icon/camera_off.png"
#define ICON_CAMERA_ON       ":/images/icon/camera_on.png"
#define ICON_HOME            ":/images/icon/home.png"
#define ICON_INIT            ":/images/icon/initial.png"
#define ICON_CALIBRATION     ":/images/icon/calibration.png"
#define ICON_READ            ":/images/icon/read.png"
#define ICON_MOVE            ":/images/icon/move.png"
#define ICON_NO_IMAGE        ":/images/icon/no_image_placeholder.png"

#define POS_ERROR           0.001

//Flowchart Params
#define AA_PIECE_START "Start"
#define AA_PIECE_LOAD_CAMERA "Load Camera"
#define AA_PIECE_INIT_CAMERA "Init Camera"
#define AA_PIECE_PR_TO_BOND "PR To Bond"
#define AA_PIECE_INITIAL_TILT "Initial Tilt"
#define AA_PIECE_PICK_LENS "AA Pick Lens"
#define AA_PIECE_UNLOAD_LENS "AA Unload Lens"
#define AA_PIECE_Z_OFFSET "Z Offset"
#define AA_PIECE_AA "AA"
#define AA_PIECE_OC "OC"
#define AA_PIECE_MTF "MTF"
#define AA_PIECE_UV "UV"
#define AA_PIECE_DISPENSE "Dispense"
//#define AA_PIECE_SAVE_IMAGE "Save Image"
#define AA_PIECE_DELAY "Delay"
#define AA_PIECE_JOIN "Join"
#define AA_PIECE_SAVE_IMAGE "Save Image"
#define AA_PIECE_ACCEPT "Accept"
#define AA_PIECE_REJECT "Reject"

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

#endif
// CONFIG_H
