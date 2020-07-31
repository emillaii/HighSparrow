$(document).ready(function () {

  var data = {};
  var init_aa_params = {
    mode: 1, aa1_start_pos: 0, aa1_stop_pos: 0, aa2_start_pos: 0, aa2_stop_pos: 0, delay_in_ms: 0,
    offset_in_um: -40, delay_Z_in_ms: 200, step_size: 10,
    position_checking: 0, is_debug: 0, image_count: 7, enable_tilt: 0
  };
  var init_oc_params = { enable_motion: 1, fast_mode: 0, mode: 0, oc_intensity_threshold: 100, is_debug: 0, delay_in_ms: 0, retry: 0, is_check: 0, x_limit_in_um: 5, y_limit_in_um: 5 };
  var init_initial_tilt_params = { roll: 0, pitch: 0, delay_in_ms: 0};
  var init_basic_params = { retry: 0, delay_in_ms: 200 };
  var init_y_level_params = { enable_plot: 1, min: 0, max: 200, mode: 0, margin: 100 };
  var init_uv_params = {time_in_ms: 3000, enable_OTP: 0, enable_y_level_check: 0, margin: 5, min: 0, max: 200, change_allowance: 2, y_level_path: 0, delay_before_check: 500 };
  var init_z_offset = { type: 0, z_offset_in_um_aa1: 0, z_offset_in_um_aa2: 0, delay_in_ms: 0 };
  var init_xy_offset = { type: 0, x_offset_in_um_aa1: 0, y_offset_in_um_aa1: 0, x_offset_in_um_aa2: 0, y_offset_in_um_aa2: 0, delay_in_ms: 0};
  var init_dispense_params = { x_offset_in_um: 0, y_offset_in_um: 0, z_offset_in_um: 0, delay_in_ms: 0, enable_glue_inspection: 0, glue_inspection_mode: 0, max_glue_width_in_mm:0, min_glue_width_in_mm: 0, max_avg_glue_width_in_mm:0 };
  var init_save_image = { type: 0, lighting: 100 };
  var init_grr_params ={ change_lens: 1, change_sensor: 0, repeat_time: 10, change_time: 11};
  var init_lens_params = { cmd: 0, target_position: 70, ois_x_target_position: 0, ois_y_target_position: 0, delay_in_ms: 0 }
  var init_partical_check_params = { enable_partical_check: 0, delay_in_ms: 0 }
  var latestCreatedLinkId;
  var $linkProperties = $('#link_properties');
  var $operatorTitle = $('#operator_title');
  var $aa_operatorTitle = $('#aa_operator_title');
  var $oc_operatorTitle = $('#oc_operator_title');
  var $initial_tilt_operatorTitle = $('#initial_tilt_operator_title');
  var $operator_properties = $('#operator_properties');
  var $aa_operator_properties = $('#aa_operator_properties');
  var $initial_tilt_properties = $('#initial_tilt_properties');
  var $oc_operator_properties = $('#oc_operator_properties');
  var $aa_z_offset_operator_properties = $('#aa_z_offset_operator_properties');
  var $aa_z_offset_operator_title = $('#aa_z_offset_operator_title');
  var $aa_xy_offset_operator_properties = $('#aa_xy_offset_operator_properties');
  var $aa_xy_offset_operator_title = $('#aa_xy_offset_operator_title');
  var $save_image_operator_properties = $('#save_image_operator_properties');
  var $save_image_operator_title = $('#save_image_operator_title');
  var $dispense_operator_properties = $('#dispense_operator_properties');
  var $dispense_operator_title = $('#dispense_operator_title');
  var $grr_operator_title = $('#grr_operator_title');
  var $grr_operator_properties = $('#grr_operator_properties');
  var $uv_operator_title = $('#uv_operator_title');
  var $uv_operator_properties = $('#uv_operator_properties');
  var $y_level_operator_title = $('#y_level_operator_title');
  var $y_level_operator_properties = $('#y_level_operator_properties');
  var $mtf_table = $('#mtf_table');
  var $linkColor = $('#link_color');
  var $init_lens_operator_title = $('#init_lens_operator_title');
  var $init_lens_operator_properties = $('#init_lens_operator_properties');
  var $partical_check_operator_title = $('#partical_check_operator_title');
  var $partical_check_operator_properties = $('#partical_check_operator_properties');

  //Init the table 
  $aa_operator_properties.append("<div style=\"margin-top:20px;\">Select AA mode: <select disabled id=\"aa_mode\" style=\"position: relative;\"><option value=2>StationaryScan</option><option value=1>DOV_Search</option><option value=0>ZScan</option><option value=3>XScan</option></select><div class=\"dropdown\"><button id=\"aa_select_mode\" class=\"dropbtn\">Select AA Mode</button><div id=\"myDropdown\" class=\"dropdown-content\"><button id=\"select_zscan_mode\" class=\"btn btn-info test_button\">ZScan Mode</button><button id=\"select_dfov_mode\" class=\"btn btn-info test_button\">DFOV Search Mode</button><button id=\"select_stationary_scan_mode\" class=\"btn btn-info test_button\">Stationary Scan Mode</button><button id=\"select_x_scan_mode\" class=\"btn btn-info test_button\">XScan Mode</button></div></div></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\" id=\"aa1_start_position_div\">AA 1 Start Position: <input type=\"number\" id=\"aa1_start_position\"></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\" id=\"aa1_stop_position_div\">AA 1 Stop Position: <input type=\"number\" id=\"aa1_stop_position\"></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\" id=\"aa2_start_position_div\">AA 2 Start Position: <input type=\"number\" id=\"aa2_start_position\"></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\" id=\"aa2_stop_position_div\">AA 2 Stop Position: <input type=\"number\" id=\"aa2_stop_position\"></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\">Step Size in um: <input type=\"number\" id=\"aa_step_size\"></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\" id=\"aa_offset_in_um_div\">AA Offset in um: <input type=\"number\" id=\"aa_offset_in_um\"></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\">Delay in ms: <input type=\"number\" id=\"aa_delay_Z_in_ms\"></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\">Position Checking: <select id=\"aa_position_checking\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\">Image Count: <input type=\"number\" id=\"aa_image_count\"></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px;\">Enable Tilt:  <select id=\"aa_enable_tilt\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $aa_operator_properties.append("<div style=\"margin-top:20px\">Delay: <input type=\"number\" id=\"aa_delay_in_ms\"></div>");

  $initial_tilt_properties.append("<div style=\"margin-top:20px\">Roll: <input type=\"number\" id=\"roll\"></div>");
  $initial_tilt_properties.append("<div style=\"margin-top:20px\">Pitch: <input type=\"number\" id=\"pitch\"></div>");
  $initial_tilt_properties.append("<div style=\"margin-top:20px\">Delay: <input type=\"number\" id=\"initial_tilt_delay_in_ms\"></div>");
  
  $y_level_operator_properties.append("<div style=\"margin-top:20px;\">Enable Plot:  <select id=\"y_level_enable_plot\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $y_level_operator_properties.append("<div style=\"margin-top:20px\">Min Y Level: <input type=\"number\" id=\"y_level_min\"></div>");
  $y_level_operator_properties.append("<div style=\"margin-top:20px\">Max Y Level: <input type=\"number\" id=\"y_level_max\"></div>");
  $y_level_operator_properties.append("<div style=\"margin-top:20px;\">Path mode:  <select id=\"y_level_mode\" size=\"2\"><option value=0>Diagonal</option><option value=1>Rectangle</option></select></div>");
  $y_level_operator_properties.append("<div style=\"margin-top:20px\">Rectangle Margin: <input type=\"number\" id=\"y_level_margin\"></div>");

  $operator_properties.append("<div style=\"margin-top:20px\">Retry Count: <input type=\"number\" id=\"basic_retry\"></div>");
  $operator_properties.append("<div style=\"margin-top:20px\">Delay: <input type=\"number\" id=\"basic_delay\"></div>");

  $oc_operator_properties.append("<div style=\"margin-top:20px\">Enable Motion: <select id=\"oc_enable_motion\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $oc_operator_properties.append("<div style=\"margin-top:20px\">Fast Mode: <select id=\"oc_fast_mode\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $oc_operator_properties.append("<div style=\"margin-top:20px\">OC Mode: <select id=\"oc_mode\" size=\"2\"><option value=0>Chart Pattern</option><option value=1>Optical Mass Center</option></select></div>");
  $oc_operator_properties.append("<div style=\"margin-top:20px\">Debug: <select id=\"oc_is_debug\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $oc_operator_properties.append("<div style=\"margin-top:20px\">Retry Count: <input type=\"number\" id=\"oc_retry\"></div>");
  $oc_operator_properties.append("<div style=\"margin-top:20px\">Delay: <input type=\"number\" id=\"oc_delay_in_ms\"></div>");
  $oc_operator_properties.append("<div style=\"margin-top:20px\">OC Check: <select id=\"oc_is_check\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $oc_operator_properties.append("<div style=\"margin-top:20px\">X limit in um: <input type=\"number\" id=\"oc_x_limit_in_um\"></div>");
  $oc_operator_properties.append("<div style=\"margin-top:20px\">Y limit in um: <input type=\"number\" id=\"oc_y_limit_in_um\"></div>");
  $oc_operator_properties.append("<div style=\"margin-top:20px\">Intensity threshold: <input type=\"number\" id=\"oc_intensity_threshold\"></div>");
  
  $dispense_operator_properties.append("<div style=\"margin-top:20px\">SUT X Offset in um: <input type=\"number\" id=\"dispense_x_offset_in_um\"></div>");
  $dispense_operator_properties.append("<div style=\"margin-top:20px\">SUT Y Offset in um: <input type=\"number\" id=\"dispense_y_offset_in_um\"></div>");
  $dispense_operator_properties.append("<div style=\"margin-top:20px\">SUT Z Offset in um: <input type=\"number\" id=\"dispense_z_offset_in_um\"></div>");
  $dispense_operator_properties.append("<div style=\"margin-top:20px\">Enable Glue Inspection: <select id=\"enable_glue_inspection\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $dispense_operator_properties.append("<div style=\"margin-top:20px\">Glue Inspection Mode: <select id=\"glue_inspection_mode\" size=\"2\"><option value=0>Manual</option><option value=1>Auto</option></select></div>");
  $dispense_operator_properties.append("<div style=\"margin-top:20px\">Min Glue Width in mm: <input type=\"number\" id=\"min_glue_width_in_mm\"></div>");
  $dispense_operator_properties.append("<div style=\"margin-top:20px\">Max Glue Width in mm: <input type=\"number\" id=\"max_glue_width_in_mm\"></div>");
  $dispense_operator_properties.append("<div style=\"margin-top:20px\">Max Avg Glue Width in mm: <input type=\"number\" id=\"max_avg_glue_width_in_mm\"></div>");
  $dispense_operator_properties.append("<div style=\"margin-top:20px\">Delay: <input type=\"number\" id=\"dispense_delay_in_ms\"></div>");
  
  $aa_z_offset_operator_properties.append("<div style=\"margin-top:20px\">Motion Type: <select id=\"aa_z_offset_type\"><option value=0>SUT Z</option><option value=1>AA Z</option></select></div>");
  $aa_z_offset_operator_properties.append("<div style=\"margin-top:20px\">AA 1 Z Offset in um: <input type=\"number\" id=\"z_offset_in_um_aa1\"></div>");
  $aa_z_offset_operator_properties.append("<div style=\"margin-top:20px\">AA 2 Z Offset in um: <input type=\"number\" id=\"z_offset_in_um_aa2\"></div>");
  $aa_z_offset_operator_properties.append("<div style=\"margin-top:20px\">Delay: <input type=\"number\" id=\"aa_z_offset_delay_in_ms\"></div>");

  $aa_xy_offset_operator_properties.append("<div style=\"margin-top:20px\">Motion Type: <select id=\"aa_xy_offset_type\"><option value=0>SUT</option><option value=1>AA</option></select></div>");
  $aa_xy_offset_operator_properties.append("<div style=\"margin-top:20px\">AA 1 X Offset in um: <input type=\"number\" id=\"aa_x_offset_in_um_aa1\"></div>");
  $aa_xy_offset_operator_properties.append("<div style=\"margin-top:20px\">AA 1 Y Offset in um: <input type=\"number\" id=\"aa_y_offset_in_um_aa1\"></div>");
  $aa_xy_offset_operator_properties.append("<div style=\"margin-top:20px\">AA 2 X Offset in um: <input type=\"number\" id=\"aa_x_offset_in_um_aa2\"></div>");
  $aa_xy_offset_operator_properties.append("<div style=\"margin-top:20px\">AA 2 Y Offset in um: <input type=\"number\" id=\"aa_y_offset_in_um_aa2\"></div>");
  $aa_xy_offset_operator_properties.append("<div style=\"margin-top:20px\">Delay: <input type=\"number\" id=\"aa_xy_offset_delay_in_ms\"></div>");
  
  $save_image_operator_properties.append("<div style=\"margin-top:20px\">Camera Type: <select id=\"save_image_type\"><option value=0>Downlook Camera</option><option value=1>Uplook Camera</option><option value=2>Pickarm Camera</option></select></div>");
  $save_image_operator_properties.append("<div style=\"margin-top:20px\">Lighting: <input type=\"number\" id=\"save_image_lighting\" value=100></div>");
  
  $grr_operator_properties.append("<div style=\"margin-top:20px\">Change Lens: <select id=\"grr_change_lens\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $grr_operator_properties.append("<div style=\"margin-top:20px\">Change Sensor: <select id=\"grr_change_sensor\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $grr_operator_properties.append("<div style=\"margin-top:20px\">Repeat Time: <input type=\"number\" id=\"grr_repeat_time\" value=10></div>");
  $grr_operator_properties.append("<div style=\"margin-top:20px\">Change Time: <input type=\"number\" id=\"grr_change_time\" value=10></div>");
  
  $uv_operator_properties.append("<div style=\"margin-top:20px\">Enable OTP: <select id=\"uv_enable_OTP\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $uv_operator_properties.append("<div style=\"margin-top:20px\">UV time: <input type=\"number\" id=\"uv_time_in_ms\" value=3000></div>"); 
  $uv_operator_properties.append("<div style=\"margin-top:20px\">Enable Y_Level check: <select id=\"uv_enable_y_level_check\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $uv_operator_properties.append("<div style=\"margin-top:20px\">Y Level Check Path Method: <select id=\"uv_y_level_path\" size=\"2\"><option value=0>Use Margin</option><option value=1>Custom</option></select></div>");
  $uv_operator_properties.append("<div style=\"margin-top:20px\">Delay Before Y_Level Check: <input type=\"number\" id=\"uv_delay_before_check\" value=500></div>"); 
  $uv_operator_properties.append("<div style=\"margin-top:20px\">Y_Level margin: <input type=\"number\" id=\"uv_y_level_margin\" value=5></div>"); 
  $uv_operator_properties.append("<div style=\"margin-top:20px\">Y_Level min spec: <input type=\"number\" id=\"uv_y_level_min_spec\" value=0></div>");
  $uv_operator_properties.append("<div style=\"margin-top:20px\">Y_Level max spec: <input type=\"number\" id=\"uv_y_level_max_spec\" value=200></div>");
  $uv_operator_properties.append("<div style=\"margin-top:20px\">Y_Level change allowance: <input type=\"number\" id=\"uv_y_level_change_allowance\" value=2></div>");
  
  
  $init_lens_operator_properties.append("<div style=\"margin-top:20px\">CMD: <select id=\"init_lens_cmd\" size=\"4\"><option value=0>Init</option><option value=1>AF+OIS XY Move</option><option value=2>AF Move</option><option value=3>OIS XY Move</option></select></div>");
  $init_lens_operator_properties.append("<div style=\"margin-top:20px\">Target Position: <input type=\"number\" id=\"init_lens_target_position\" value=70></div>");
  $init_lens_operator_properties.append("<div style=\"margin-top:20px\">OIS X Target Position: <input type=\"number\" id=\"init_lens_ois_x_target_position\" value=70></div>");
  $init_lens_operator_properties.append("<div style=\"margin-top:20px\">OIS Y Target Position: <input type=\"number\" id=\"init_lens_ois_y_target_position\" value=70></div>");
  $init_lens_operator_properties.append("<div style=\"margin-top:20px\">Delay: <input type=\"number\" id=\"init_lens_delay_in_ms\" value=0></div>"); 
  
  $partical_check_operator_properties.append("<div style=\"margin-top:20px\">Enable partical check: <select id=\"enable_partical_check\" size=\"2\"><option value=0>False</option><option value=1>True</option></select></div>");
  $partical_check_operator_properties.append("<div style=\"margin-top:20px\">Delay in ms: <input type=\"number\" id=\"partical_check_delay_in_ms\"></div>");
  
  // Apply the plugin on a standard, empty div...
  var $flowchart = $('#example_7');
  $mtf_table.hide();

  $flowchart.flowchart({
    data: data,
    multipleLinksOnInputof: true,
    multipleLinksOnOutput: true,
    linkWidth: 1,
    onOperatorSelect: function (operatorId) {
      var params = $flowchart.flowchart('getOperatorParams', operatorId);
      $operator_properties.hide();
      $aa_operator_properties.hide();
      $initial_tilt_properties.hide();
      $oc_operator_properties.hide();
      $aa_z_offset_operator_properties.hide();
      $aa_xy_offset_operator_properties.hide();
	  $save_image_operator_properties.hide();
	  $dispense_operator_properties.hide();
	  $grr_operator_properties.hide();
	  $mtf_table.hide();
	  $y_level_operator_properties.hide();
	  $uv_operator_properties.hide();
	  $init_lens_operator_properties.hide();
	  $partical_check_operator_properties.hide();
      if (operatorId.includes("AA_")) {
        $aa_operator_properties.show();
        $aa_operatorTitle.val($flowchart.flowchart('getOperatorTitle', operatorId));
        $('#aa_mode').val(params["mode"]);
        $('#aa1_start_position').val(params["aa1_start_pos"]);
        $('#aa1_stop_position').val(params["aa1_stop_pos"]);
		$('#aa2_start_position').val(params["aa2_start_pos"]);
        $('#aa2_stop_position').val(params["aa2_stop_pos"]);
        $('#aa_step_size').val(params["step_size"]);
        $('#aa_offset_in_um').val(params["offset_in_um"]);
        $('#aa_delay_Z_in_ms').val(params["delay_Z_in_ms"]);
        $('#aa_position_checking').val(params["position_checking"]);
        $('#aa_edge_filter').val(params["edge_filter"]);
        $('#aa_is_debug').val(params["is_debug"]);
        $('#aa_image_count').val(params["image_count"]);
        $('#aa_enable_tilt').val(params["enable_tilt"]);
        $('#aa_delay_in_ms').val(params["delay_in_ms"]);
      } else if (operatorId.includes("Initial Tilt")) {
        $initial_tilt_properties.show();
        $initial_tilt_operatorTitle.val($flowchart.flowchart('getOperatorTitle', operatorId));
        $('#roll').val(params["roll"]);
        $('#pitch').val(params['pitch']);
        $('#initial_tilt_delay_in_ms').val(params['delay_in_ms']);
      } else if (operatorId.includes("Y_Level")) {
		$y_level_operator_properties.show();
		$y_level_operator_title.val($flowchart.flowchart('getOperatorTitle', operatorId));
        $('#y_level_enable_plot').val(params["enable_plot"]);
		$('#y_level_min').val(params["min"]);
		$('#y_level_max').val(params["max"]);
		$('#y_level_mode').val(params["mode"]);
		$('#y_level_margin').val(params["margin"]);
	  } else if (operatorId.includes("Z Offset")) {
        $aa_z_offset_operator_properties.show();
        $aa_z_offset_operator_title.val($flowchart.flowchart('getOperatorTitle', operatorId));
        $('#aa_z_offset_type').val(params["type"]);
        $('#z_offset_in_um_aa1').val(params["z_offset_in_um_aa1"]);
		$('#z_offset_in_um_aa2').val(params["z_offset_in_um_aa2"]);
        $('#aa_z_offset_delay_in_ms').val(params["delay_in_ms"]);
      } else if (operatorId.includes("XY Offset")) {
        $aa_xy_offset_operator_properties.show();
        $aa_xy_offset_operator_title.val($flowchart.flowchart('getOperatorTitle', operatorId));
        $('#aa_xy_offset_type').val(params["type"]);
        $('#aa_x_offset_in_um_aa1').val(params["x_offset_in_um_aa1"]);
        $('#aa_y_offset_in_um_aa1').val(params["y_offset_in_um_aa1"]);
		$('#aa_x_offset_in_um_aa2').val(params["x_offset_in_um_aa2"]);
        $('#aa_y_offset_in_um_aa2').val(params["y_offset_in_um_aa2"]);
        $('#aa_xy_offset_delay_in_ms').val(params["delay_in_ms"]);
      } else if (operatorId.includes("OC")) {
        $oc_operator_properties.show();
        $oc_operatorTitle.val($flowchart.flowchart('getOperatorTitle', operatorId));
        $('#oc_fast_mode').val(params["fast_mode"]);
		$('#oc_mode').val(params["mode"]);
        $('#oc_enable_motion').val(params["enable_motion"]);
        $('#oc_delay_in_ms').val(params["delay_in_ms"]);
        $('#oc_is_debug').val(params["is_debug"]);
        $('#oc_retry').val(params["retry"]);
		$('#oc_is_check').val(params["is_check"]);
		$('#oc_x_limit_in_um').val(params["x_limit_in_um"]);
		$('#oc_y_limit_in_um').val(params["y_limit_in_um"]);
		$('#oc_intensity_threshold').val(params["oc_intensity_threshold"]);
		
      } else if (operatorId.includes("Save Image")) {
		$save_image_operator_properties.show();
		$save_image_operator_title.val($flowchart.flowchart('getOperatorTitle', operatorId));
		$('#save_image_type').val(params["type"]);
		$('#save_image_lighting').val(params["lighting"]);
	  } else if (operatorId.includes("Dispense")) {
		$dispense_operator_properties.show();
		$dispense_operator_title.val($flowchart.flowchart('getOperatorTitle', operatorId));
        $('#dispense_x_offset_in_um').val(params["x_offset_in_um"]);
        $('#dispense_y_offset_in_um').val(params["y_offset_in_um"]);
        $('#dispense_z_offset_in_um').val(params["z_offset_in_um"]);
		$('#enable_glue_inspection').val(params["enable_glue_inspection"]);
		$('#glue_inspection_mode').val(params["glue_inspection_mode"]);
		$('#max_glue_width_in_mm').val(params["max_glue_width_in_mm"]);
		$('#min_glue_width_in_mm').val(params["min_glue_width_in_mm"]);
		$('#max_avg_glue_width_in_mm').val(params["max_avg_glue_width_in_mm"]);
        $('#dispense_delay_in_ms').val(params["delay_in_ms"]);
	  } else if (operatorId.includes("MTF")) {
		$mtf_table.show();
		console.log(params);
		init_mtf_table();
		$('#mtf_operator_title').val($flowchart.flowchart('getOperatorTitle', operatorId));
		Object.keys(params).forEach(function(key) {
			console.log('Key : ' + key + ', Value : ' + params[key])
			if (key == "SFR_DEV_TOL" || key == "CC_MIN" || key == "CC_MAX" || key == "L1_MIN" || key == "L1_MAX" || key == "L2_MIN" || key == "L2_MAX" || key == "L3_MIN" || key == "L3_MAX" || key == "CC_AVG_MIN" || key == "CC_AVG_MAX" || key == "L1_AVG_MIN" || key == "L1_AVG_MAX" || key == "L2_AVG_MIN" || key == "L2_AVG_MAX" || key == "L3_AVG_MIN" || key == "L3_AVG_MAX"){
				var table = document.getElementById("mtf_table_form");
				var row = table.insertRow(table.rows.length);
				var row_number = table.rows.length;
				var cell1 = row.insertCell(0);
				var cell2 = row.insertCell(1);
				var cell3 = row.insertCell(2);
				var select_position = document.createElement('select');
				var sfr_score = document.createElement('input');
				select_position.innerHTML = "<option value=SFR_DEV_TOL>SFR_DEV_TOL</option><option value=CC_MIN>CC_MIN</option><option value=CC_MAX>CC_MAX</option><option value=L1_MIN>L1_MIN</option><option value=L1_MAX>L1_MAX</option><option value=L2_MIN>L2_MIN</option><option value=L2_MAX>L2_MAX</option><option value=L3_MIN>L3_MIN</option><option value=L3_MAX>L3_MAX</option><option value=CC_AVG_MIN>CC_AVG_MIN</option><option value=CC_AVG_MAX>CC_AVG_MAX</option><option value=L1_AVG_MIN>L1_AVG_MIN</option><option value=L1_AVG_MAX>L1_AVG_MAX</option><option value=L2_AVG_MIN>L2_AVG_MIN</option><option value=L2_AVG_MAX>L2_AVG_MAX</option><option value=L3_AVG_MIN>L3_AVG_MIN</option><option value=L3_AVG_MAX>L3_AVG_MAX</option>";
				select_position.value = key;
				cell1.appendChild(select_position);
				sfr_score.value = params[key]; sfr_score.type="number"; 
				sfr_score.min=0; sfr_score.max=100;
				cell2.appendChild(sfr_score);
				var delete_btn = document.createElement('button');
				delete_btn.className = "btn-danger";
				delete_btn.value = "-";
				delete_btn.onclick = function() {
					table.deleteRow(row_number-1);
				};
				cell3.appendChild(delete_btn);
			}
		})
	  }else if (operatorId.includes("GRR")) {
		$grr_operator_properties.show();
		$grr_operator_title.val($flowchart.flowchart('getOperatorTitle', operatorId));
		$('#grr_change_lens').val(params["change_lens"]);
		$('#grr_change_sensor').val(params["change_sensor"]);
        $('#grr_repeat_time').val(params["repeat_time"]);
        $('#grr_change_time').val(params["change_time"]); 
	  }else if (operatorId.includes("UV")) {
		$uv_operator_properties.show();
		$uv_operator_title.val($flowchart.flowchart('getOperatorTitle', operatorId));
		$('#uv_time_in_ms').val(params["time_in_ms"]);
        $('#uv_enable_OTP').val(params["enable_OTP"]);
		$('#uv_enable_y_level_check').val(params["enable_y_level_check"]);
		$('#uv_y_level_path').val(params["y_level_path"]);
		$('#uv_delay_before_check').val(params["delay_before_check"]);
		$('#uv_y_level_margin').val(params["margin"]);
		$('#uv_y_level_min_spec').val(params["min"]);
		$('#uv_y_level_max_spec').val(params["max"]);
		$('#uv_y_level_change_allowance').val(params["change_allowance"]);		
	  }else if (operatorId.includes("Init_Lens")) {
		$init_lens_operator_properties.show();
		$init_lens_operator_title.val($flowchart.flowchart('getOperatorTitle', operatorId));
        $('#init_lens_cmd').val(params["cmd"]);
		$('#init_lens_ois_x_target_position').val(params["ois_x_target_position"]);
		$('#init_lens_ois_y_target_position').val(params["ois_y_target_position"]);
        $('#init_lens_target_position').val(params["target_position"]);
		$('#init_lens_delay_in_ms').val(params["delay_in_ms"]);
	  }else if (operatorId.includes("Partical Check")) {
		$partical_check_operator_properties.show();
		$partical_check_operator_title.val($flowchart.flowchart('getOperatorTitle', operatorId));
		$('#enable_partical_check').val(params["enable_partical_check"]);
		$('#partical_check_delay_in_ms').val(params["delay_in_ms"]);
	  }	  
	  else {
        $operator_properties.show();
        $operatorTitle.val($flowchart.flowchart('getOperatorTitle', operatorId));
        $('#basic_delay').val(params["delay_in_ms"]);
        $('#basic_retry').val(params['retry']);
      }
      return true;
    },
    onOperatorUnselect: function () {
      $operator_properties.hide();
      $aa_operator_properties.hide();
      $initial_tilt_properties.hide();
      $aa_z_offset_operator_properties.hide();
      $aa_xy_offset_operator_properties.hide();
	  $oc_operator_properties.hide();
	  $dispense_operator_properties.hide();
	  $save_image_operator_properties.hide();
	  $grr_operator_properties.hide();
	  $mtf_table.hide();
	  $y_level_operator_properties.hide();
	  $uv_operator_properties.hide();
	  $init_lens_operator_properties.hide();
	  $partical_check_operator_properties.hide();
      return true;
    },
	onOperatorCreate: function (operatorId, operatorData, fullElement) {
		console.log(operatorId);
		operatorI++;
		return true;
	},
    onLinkSelect: function (linkId) {
      return true;
    },
    onLinkUnselect: function () {
      $linkProperties.hide();
      return true;
    },
    onLinkCreate: function (linkId, linkData) {
      console.log(linkId + " link is created. " + linkData);
      if (linkData.fromConnector == "fail") {
        latestCreatedLinkId = linkId;
      }
      return true;
    },
    onAfterChange: function (changeType) {
      if (changeType == "link_create" && typeof latestCreatedLinkId !== 'undefined') {
        var color = "#FF3371";
        console.log("latestCreatedLinkId: " + latestCreatedLinkId);
        $flowchart.flowchart('setLinkMainColor', latestCreatedLinkId, color);
        $flowchart.flowchart('redrawLinksLayer');
      }
      return true;
    }, 
	onOperatorDoubleClicked: function (operatorId) {
		var params = $flowchart.flowchart('getOperatorParams', operatorId);
		params["operatorId"] = operatorId;
		console.log("onOperatorDoubleClicked: " + operatorId);
		$('#flowchart_running_cmd').val(JSON.stringify(params, null, 2));
		return true;
	}
  });

  $operatorTitle.keyup(function () {
    var selectedOperatorId = $flowchart.flowchart('getSelectedOperatorId');
    if (selectedOperatorId != null) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $operatorTitle.val());
    }
  });

  $linkColor.change(function () {
    var selectedLinkId = $flowchart.flowchart('getSelectedLinkId');
    if (selectedLinkId != null) {
      $flowchart.flowchart('setLinkMainColor', selectedLinkId, $linkColor.val());
    }
  });

  var operatorI = 0;
  function duplicateOperationWidget() {
	var selectedOperatorId = $flowchart.flowchart('getSelectedOperatorId');
	var nameIndex = selectedOperatorId.indexOf('_');
	var name = selectedOperatorId.substring(0, nameIndex);
	console.log("New operator: " + name);
	var operatorId = name + '_' + operatorI;
    var operatorData = {
      top: 10,
      left: 10,
      properties: {
        title: name,
        inputs: {
          input_1: {
            label: 'In',
            multiple: false
          }
        },
        outputs: {
          success: {
            label: 'success',
          },
          fail: {
            label: 'fail',
          }
        }
      }
    };
    console.log("Create operator");
    $flowchart.flowchart('createOperator', operatorId, operatorData);
    if (selectedOperatorId.includes("AA_")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $aa_operatorTitle.val());
      var params = {
        mode: Number($('#aa_mode').val()),
		aa1_start_pos: Number($('#aa1_start_position').val()),
		aa1_stop_pos: Number($('#aa1_stop_position').val()),
		aa2_start_pos: Number($('#aa2_start_position').val()),
		aa2_stop_pos: Number($('#aa2_stop_position').val()),
        step_size: Number($('#aa_step_size').val()), 
		offset_in_um: Number($('#aa_offset_in_um').val()),
		delay_Z_in_ms: Number($('#aa_delay_Z_in_ms').val()),
        image_count: Number($('#aa_image_count').val()),
        enable_tilt: Number($('#aa_enable_tilt').val()),
        position_checking: Number($('#aa_position_checking').val()), 
		edge_filter: Number($('#aa_edge_filter').val()), 
		is_debug: Number($('#aa_is_debug').val()),
		delay_in_ms: Number($('#aa_delay_in_ms').val())
      };

	  $flowchart.flowchart('setOperatorParams', operatorId, params);
    } else if (selectedOperatorId.includes("Initial Tilt")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $initial_tilt_operatorTitle.val());
      var params = { roll: Number($('#roll').val()),
	  pitch: Number($('#pitch').val()),
	  delay_in_ms:Number($('#initial_tilt_delay_in_ms'))
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
    } else if (selectedOperatorId.includes("OC")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $oc_operatorTitle.val());
      var params = {
        enable_motion: Number($('#oc_enable_motion').val()),
        fast_mode: Number($('#oc_fast_mode').val()),
		mode: Number($('#oc_mode').val()),
		is_debug: Number($('#oc_is_debug').val()),
        delay_in_ms: Number($('#oc_delay_in_ms').val()),
		retry: Number($('#oc_retry').val()),
		is_check: Number($('#oc_is_check').val()),
		x_limit_in_um: Number($('#oc_x_limit_in_um').val()),
		y_limit_in_um: Number($('#oc_y_limit_in_um').val()),
		oc_intensity_threshold: Number($('#oc_intensity_threshold').val())
      };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
    } else if (selectedOperatorId.includes("Z Offset")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $aa_z_offset_operator_title.val());
      var params = { type: Number($('#aa_z_offset_type').val()),
	  z_offset_in_um_aa1: Number($('#z_offset_in_um_aa1').val()),
	  z_offset_in_um_aa2: Number($('#z_offset_in_um_aa2').val()),
	  delay_in_ms: Number($('#aa_z_offset_delay_in_ms').val())
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
    } else if (selectedOperatorId.includes("XY Offset")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $aa_xy_offset_operator_title.val());
      var params = { type: Number($('#aa_xy_offset_type').val()),
	  x_offset_in_um_aa1: Number($('#aa_x_offset_in_um_aa1').val()),
	  y_offset_in_um_aa1: Number($('#aa_y_offset_in_um_aa1').val()),
	  x_offset_in_um_aa2: Number($('#aa_x_offset_in_um_aa2').val()),
	  y_offset_in_um_aa2: Number($('#aa_y_offset_in_um_aa2').val()),
	  delay_in_ms: Number($('#aa_xy_offset_delay_in_ms').val())
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
    } else if (selectedOperatorId.includes("Save Image")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $save_image_operator_title.val());
      var params = { type: Number($('#save_image_type').val()),
	  lighting: $('#save_image_lighting').val()
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
	} else if (selectedOperatorId.includes("Dispense")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $dispense_operator_title.val());
      var params = {x_offset_in_um:  Number($('#dispense_x_offset_in_um').val()),
	  y_offset_in_um:  Number($('#dispense_y_offset_in_um').val()),
	  z_offset_in_um:  Number($('#dispense_z_offset_in_um').val()),
	  enable_glue_inspection: Number($('#enable_glue_inspection').val()),
	  glue_inspection_mode: Number($('#glue_inspection_mode').val()),
	  min_glue_width_in_mm: Number($('#min_glue_width_in_mm').val()),
	  max_glue_width_in_mm: Number($('#max_glue_width_in_mm').val()),
	  max_avg_glue_width_in_mm: Number($('#max_avg_glue_width_in_mm').val()),
	  delay_in_ms:  Number($('#dispense_delay_in_ms').val())
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
	} else if (selectedOperatorId.includes("MTF")) {
	  getTableData();
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#mtf_operator_title').val());
      var params = getTableData();
      $flowchart.flowchart('setOperatorParams', operatorId, params);
	} else if (selectedOperatorId.includes("GRR")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#grr_operator_title').val());
      var params = { change_lens: Number($('#grr_change_lens').val()),
	  change_sensor: Number($('#grr_change_sensor').val()),
	  repeat_time:  Number($('#grr_repeat_time').val()), 
	  change_time:  Number($('#grr_change_time').val())};
      $flowchart.flowchart('setOperatorParams', operatorId, params);
	} else if (selectedOperatorId.includes("Y_Level")){
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#y_level_operator_title').val());
	  var params = { enable_plot: Number($('#y_level_enable_plot').val()),
	  min: Number($('#y_level_min').val()),
	  max: Number($('#y_level_max').val()),
	  mode: Number($('#y_level_mode').val()),
	  margin: Number($('#y_level_margin').val())
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
	} else if (selectedOperatorId.includes("UV")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#uv_operator_title').val());
	  var params = { time_in_ms: Number($('#uv_time_in_ms').val()),
	  enable_OTP:  Number($('#uv_enable_OTP').val()),
	  enable_y_level_check: Number($('#uv_enable_y_level_check').val()),
	  y_level_path: Number($('#uv_y_level_path').val()),
	  delay_before_check: Number($('#uv_delay_before_check').val()),
	  margin: Number($('#uv_y_level_margin').val()),
	  min: Number($('#uv_y_level_min_spec').val()),
	  max: Number($('#uv_y_level_max_spec').val()),
	  change_allowance: Number($('#uv_y_level_change_allowance').val())
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
    } else if (selectedOperatorId.includes("Init_Lens")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#init_lens_operator_title').val());
	  var params = { delay_in_ms: Number($('#init_lens_delay_in_ms').val()),
	  cmd: Number($('#init_lens_cmd').val()),
	  ois_x_target_position:  Number($('#init_lens_ois_x_target_position').val()),
	  ois_y_target_position:  Number($('#init_lens_ois_y_target_position').val()),
	  target_position:  Number($('#init_lens_target_position').val())
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
    } else if (selectedOperatorId.includes("Partical Check")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#Partical_check_operator_title').val());
	  var params = { delay_in_ms: Number($('#partical_check_delay_in_ms').val()),
	  enable_partical_check: Number($('#enable_partical_check').val()),
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
    } else {
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, init_basic_params);
      var params = { retry: Number($('#basic_retry').val()),
	  delay_in_ms: Number($('#basic_delay').val())
	  };
      $flowchart.flowchart('setOperatorParams', operatorId, params);
    }
  }
  
  function addOperationWidget(name) {
    var operatorId = name + '_' + operatorI;
    var operatorData = {
      top: 10,
      left: 10,
      properties: {
        title: name,
        inputs: {
          input_1: {
            label: 'In',
            multiple: false
          }
        },
        outputs: {
          success: {
            label: 'success',
          },
          fail: {
            label: 'fail',
          }
        }
      }
    };
    console.log("Create operator");
    $flowchart.flowchart('createOperator', operatorId, operatorData);
    if (operatorId.includes("AA_")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_aa_params);
    } else if (operatorId.includes("Initial Tilt")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_initial_tilt_params);
    } else if (operatorId.includes("OC")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_oc_params);
    } else if (operatorId.includes("Dispense")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_dispense_params);
    } else if (operatorId.includes("Z Offset")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_z_offset);
    } else if (operatorId.includes("XY Offset")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_xy_offset);
    } else if (operatorId.includes("Save Image")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_save_image);
	} else if (operatorId.includes("Y_Level")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_y_level_params);
	}else if (operatorId.includes("GRR")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_grr_params);
	}else if (operatorId.includes("UV")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_uv_params);
	}else if (operatorId.includes("Init_Lens")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_lens_params);
	}else if (operatorId.includes("Partical Check")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_partical_check_params);
	}
	else {
      $flowchart.flowchart('setOperatorParams', operatorId, init_basic_params);
    }
  }
  
  function addMultipleOperationWidget(name) {
    var operatorId = name + '_' + operatorI;
    var operatorData = {
      top: 10,
      left: 10,
      properties: {
        title: name,
        inputs: {
          input_1: {
            label: 'In',
            multiple: true
          }
        },
        outputs: {
          success: {
            label: 'success',
          },
          fail: {
            label: 'fail',
          }
        }
      }
    };
    console.log("Create operator");
    $flowchart.flowchart('createOperator', operatorId, operatorData);
    if (operatorId.includes("AA_")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_aa_params);
    } else if (operatorId.includes("Initial Tilt")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_initial_tilt_params);
    } else if (operatorId.includes("OC")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_oc_params);
    } else if (operatorId.includes("Dispense")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_dispense_params);
    } else if (operatorId.includes("Z Offset")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_z_offset);
    } else if (operatorId.includes("XY Offset")) {
      $flowchart.flowchart('setOperatorParams', operatorId, init_xy_offset);
    } else if (operatorId.includes("Save Image")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_save_image);
	} else if (operatorId.includes("Y_Level")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_y_level_params);
	} else if (operatorId.includes("MTF")) {
	  var params = { SFR_DEV_TOL: 100, CC_MIN: 0, CC_MAX: 100, "L1_MIN": 0, "L1_MAX": 100, "L2_MIN": 0, "L2_MAX": 100, "L3_MIN": 0, "L3_MAX": 100, CC_AVG_MIN: 0, CC_AVG_MAX: 100, "L1_AVG_MIN": 0, "L1_AVG_MAX": 100, "L2_AVG_MIN": 0, "L2_AVG_MAX": 100, "L3_AVG_MIN": 0, "L3_AVG_MAX": 100}
	  $flowchart.flowchart('setOperatorParams', operatorId, params);
	}else if (operatorId.includes("GRR")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_grr_params);
	}else if (operatorId.includes("UV")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_uv_params);
	}else if (operatorId.includes("Init_Lens")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_lens_params);
	}else if (operatorId.includes("Partical Check")) {
	  $flowchart.flowchart('setOperatorParams', operatorId, init_partical_check_params);
	}
	else {
      $flowchart.flowchart('setOperatorParams', operatorId, init_basic_params);
    }
  }
  
  function addThreadWidget(name) {
    var operatorId = name + '_' + operatorI;
    var operatorData = {
      top: 10, left: 10,
      properties: {
        title: name,
        inputs: {
          input_1: {
            label: 'In',
            multiple: false
          }
        },
        outputs: {
          thread_1: {
            label: 'thread_1',
          },
          thread_2: {
            label: 'thread_2',
          }
        }
      }
    };
    $flowchart.flowchart('createOperator', operatorId, operatorData);
  }
  
  function addThreadInputAutoAddWidget(name) {
    var operatorId = name + '_' + operatorI;
    var operatorData = {
      top: 10, left: 10,
      properties: {
        title: name,
        inputs: {
          input_1: {
            label: 'In',
            multiple: true
          }
        },
        outputs: {
          thread_1: {
            label: 'thread_1',
          },
          thread_2: {
            label: 'thread_2',
          }
        }
      }
    };
    $flowchart.flowchart('createOperator', operatorId, operatorData);
  }
  
  function addThreadWidget(name) {
    var operatorId = name + '_' + operatorI;
    var operatorData = {
      top: 10, left: 10,
      properties: {
        title: name,
        inputs: {
          input_1: {
            label: 'In',
            multiple: false
          }
        },
        outputs: {
          thread_1: {
            label: 'thread_1',
          },
          thread_2: {
            label: 'thread_2',
          }
        }
      }
    };
    $flowchart.flowchart('createOperator', operatorId, operatorData);
  }

  function addJoinThreadWidget(name) {
    var operatorId = name + '_' + operatorI;
    var operatorData = {
      top: 10, left: 10,
      properties: {
        title: name,
        inputs: {
          input_1: {
            label: 'In'
          },
          input_2: {
            label: 'In'
          }
        },
        outputs: {
          success: {
            label: 'success',
          },
          fail: {
            label: 'fail',
          }
        }
      }
    };
    $flowchart.flowchart('createOperator', operatorId, operatorData);
  }

  function addStartWidget() {
    var operatorId = 'start';
    var operatorData = {
      top: 10,
      left: 10,
      properties: {
        title: 'Start',
        outputs: {
          success: {
            label: 'To'
          }
        }
      }
    };
    $flowchart.flowchart('createOperator', operatorId, operatorData);
  }

  function addEndWidget(name) {
    var operatorId = name + '_' + operatorI;
    var operatorData = {
      top: 10,
      left: 10,
      properties: {
        title: name,
        inputs: {
          input_1: {
            label: 'In',
            multiple: true
          }
        }
      }
    };
    $flowchart.flowchart('createOperator', operatorId, operatorData);
    $flowchart.flowchart('setOperatorParams', operatorId, init_basic_params);
  }

  $flowchart.siblings('.set_data').click(function () {
    var data = JSON.parse($('#flowchart_data').val());
    $flowchart.flowchart('setData', data);
  });
  
  $flowchart.siblings('.update_data').click(function () {
    var selectedOperatorId = $flowchart.flowchart('getSelectedOperatorId');
    if (selectedOperatorId == null) { return; }
    console.log('update opeator: ' + selectedOperatorId);
    if (selectedOperatorId.includes("AA_")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $aa_operatorTitle.val());
      var params = {
        mode: Number($('#aa_mode').val()),
		aa1_start_pos: Number($('#aa1_start_position').val()),
		aa1_stop_pos: Number($('#aa1_stop_position').val()),
		aa2_start_pos: Number($('#aa2_start_position').val()),
		aa2_stop_pos: Number($('#aa2_stop_position').val()),
        step_size: Number($('#aa_step_size').val()), 
		offset_in_um: Number($('#aa_offset_in_um').val()),
		delay_Z_in_ms: Number($('#aa_delay_Z_in_ms').val()),
        image_count: Number($('#aa_image_count').val()),
        enable_tilt: Number($('#aa_enable_tilt').val()),
        position_checking: Number($('#aa_position_checking').val()), 
		edge_filter: Number($('#aa_edge_filter').val()), 
		is_debug: Number($('#aa_is_debug').val()),
		delay_in_ms: Number($('#aa_delay_in_ms').val())
      };

      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("Initial Tilt")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $initial_tilt_operatorTitle.val());
      var params = { roll: Number($('#roll').val()),
	  pitch: Number($('#pitch').val()),
	  delay_in_ms:Number($('#initial_tilt_delay_in_ms'))
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("OC")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $oc_operatorTitle.val());
      var params = {
        enable_motion: Number($('#oc_enable_motion').val()),
        fast_mode: Number($('#oc_fast_mode').val()),
	    mode: Number($('#oc_mode').val()),
		is_debug: Number($('#oc_is_debug').val()),
        delay_in_ms: Number($('#oc_delay_in_ms').val()),
		retry: Number($('#oc_retry').val()),
		is_check: Number($('#oc_is_check').val()),
		x_limit_in_um: Number($('#oc_x_limit_in_um').val()),
		y_limit_in_um: Number($('#oc_y_limit_in_um').val()),
		oc_intensity_threshold: Number($('oc_intensity_threshold').val())
      };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("Z Offset")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $aa_z_offset_operator_title.val());
      var params = { type: Number($('#aa_z_offset_type').val()),
	  z_offset_in_um_aa1: Number($('#z_offset_in_um_aa1').val()),
	  z_offset_in_um_aa2: Number($('#z_offset_in_um_aa2').val()),
	  delay_in_ms: Number($('#aa_z_offset_delay_in_ms').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("XY Offset")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $aa_xy_offset_operator_title.val());
      var params = { type: Number($('#aa_xy_offset_type').val()),
	  x_offset_in_um_aa1: Number($('#aa_x_offset_in_um_aa1').val()),
	  y_offset_in_um_aa1: Number($('#aa_y_offset_in_um_aa1').val()),
	  x_offset_in_um_aa2: Number($('#aa_x_offset_in_um_aa2').val()),
	  y_offset_in_um_aa2: Number($('#aa_y_offset_in_um_aa2').val()),
	  delay_in_ms: Number($('#aa_xy_offset_delay_in_ms').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("Save Image")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $save_image_operator_title.val());
      var params = { type: Number($('#save_image_type').val()),
	  lighting: $('#save_image_lighting').val()
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("Dispense")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $dispense_operator_title.val());
      var params = {x_offset_in_um:  Number($('#dispense_x_offset_in_um').val()),
	  y_offset_in_um:  Number($('#dispense_y_offset_in_um').val()),
	  z_offset_in_um:  Number($('#dispense_z_offset_in_um').val()),
	  enable_glue_inspection: Number($('#enable_glue_inspection').val()),
	  glue_inspection_mode: Number($('#glue_inspection_mode').val()),
	  min_glue_width_in_mm: Number($('#min_glue_width_in_mm').val()),
	  max_glue_width_in_mm: Number($('#max_glue_width_in_mm').val()),
	  max_avg_glue_width_in_mm: Number($('#max_avg_glue_width_in_mm').val()),
	  delay_in_ms:  Number($('#dispense_delay_in_ms').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("MTF")) {
	  getTableData();
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#mtf_operator_title').val());
      var params = getTableData();
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("GRR")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#grr_operator_title').val());
      var params = { change_lens: Number($('#grr_change_lens').val()),
	  change_sensor: Number($('#grr_change_sensor').val()),
	  repeat_time:  Number($('#grr_repeat_time').val()), 
	  change_time:  Number($('#grr_change_time').val())};
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("Y_Level")){
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#y_level_operator_title').val());
	  var params = { enable_plot: Number($('#y_level_enable_plot').val()),
	  min: Number($('#y_level_min').val()),
	  max: Number($('#y_level_max').val()),
	  mode: Number($('#y_level_mode').val()),
	  margin: Number($('#y_level_margin').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("UV")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#uv_operator_title').val());
	  var params = { time_in_ms: Number($('#uv_time_in_ms').val()),
	  enable_OTP:  Number($('#uv_enable_OTP').val()),
	  enable_y_level_check: Number($('#uv_enable_y_level_check').val()),
	  y_level_path: Number($('#uv_y_level_path').val()),
	  delay_before_check: Number($('#uv_delay_before_check').val()),
	  margin: Number($('#uv_y_level_margin').val()),
	  min: Number($('#uv_y_level_min_spec').val()),
	  max: Number($('#uv_y_level_max_spec').val()),
	  change_allowance: Number($('#uv_y_level_change_allowance').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("Init_Lens")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#init_lens_operator_title').val());
	  var params = { delay_in_ms: Number($('#init_lens_delay_in_ms').val()),
	  cmd: Number($('#init_lens_cmd').val()),
	  ois_x_target_position: Number($('#init_lens_ois_x_target_position').val()),
	  ois_y_target_position: Number($('#init_lens_ois_y_target_position').val()),
	  target_position:  Number($('#init_lens_target_position').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    }else if (selectedOperatorId.includes("Partical Check")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#partical_check_operator_title').val());
	  var params = { delay_in_ms: Number($('#partical_check_delay_in_ms').val()),
	  enable_partical_check: Number($('#enable_partical_check').val()),
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else {
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, init_basic_params);
      var params = { retry: Number($('#basic_retry').val()),
	  delay_in_ms: Number($('#basic_delay').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    }
    alert(selectedOperatorId + " Params is updated");
  });

  function saveFile() {
    var shape = $(".shape"),
      name = $("#filename").val() || "data-" + $.now();
    shape.data("style", shape.css(["color", "width", "height"]));
    var request = function (url, filename) {
      var file = {
        json: JSON.stringify(shape.data("style"))
      };
      return $.ajax({
        beforeSend: function (jqxhr, settings) {
          jqxhr.filename = filename;
        },
        url: url,
        type: "POST",
        contentType: "application/json",
        dataType: "json",
        data: file
      }).then(function (data, textStatus, jqxhr) {
        $("a#download").attr({
          "download": jqxhr.filename + ".json",
          "href": "data:application/json;charset=utf8;base64,"
            + window.btoa(JSON.stringify(data))
        }).get(0).click();
      }, function (jqxhr, textStatus, errorThrown) {
        console.log(textStatus, errorThrown)
      });
    };
    request("/echo/json/", name)
  };
  
  init_mtf_table();
  
  function init_mtf_table() {
	var table = document.getElementById("mtf_table_form");
	table.innerHTML = "";
	var row = table.insertRow(table.rows.length);
	var cell1 = row.insertCell(0);
	var cell2 = row.insertCell(1);
	var cell3 = row.insertCell(2);
	cell1.innerHTML = "<h4>Position</h4>";
	cell2.innerHTML = "<h4>SFR Score</h4>";
	cell3.innerHTML = "<button class=\"btn-success\" id=\"add_mtf_item\">+</button>";
	var add_mtf_item_button = document.getElementById("add_mtf_item");
	add_mtf_item_button.addEventListener("click", function() {
		var table = document.getElementById("mtf_table_form");
		var row = table.insertRow(table.rows.length);
		var row_number = table.rows.length;
		var cell1 = row.insertCell(0);
		var cell2 = row.insertCell(1);
		var cell3 = row.insertCell(2);
		cell1.innerHTML = "<select><option value=SFR_DEV_TOL>SFR_DEV_TOL</option><option value=CC_MIN>CC_MIN</option><option value=CC_MAX>CC_MAX</option><option value=L1_MIN>L1_MIN</option><option value=L1_MAX>L1_MAX</option><option value=L2_MIN>L2_MIN</option><option value=L2_MAX>L2_MAX</option><option value=L3_MIN>L3_MIN</option><option value=L3_MAX>L3_MAX</option><option value=CC_AVG_MIN>CC_AVG_MIN</option><option value=CC_AVG_MAX>CC_AVG_MAX</option><option value=L1_AVG_MIN>L1_AVG_MIN</option><option value=L1_AVG_MAX>L1_AVG_MAX</option><option value=L2_AVG_MIN>L2_AVG_MIN</option><option value=L2_AVG_MAX>L2_AVG_MAX</option><option value=L3_AVG_MIN>L3_AVG_MIN</option><option value=L3_AVG_MAX>L3_AVG_MAX</option></select>";
		cell2.innerHTML = "<input value=50 type=\"number\" min=0 max=100>";
		var delete_btn = document.createElement('button');
		delete_btn.className = "btn-danger";
		delete_btn.value = "-";
		delete_btn.onclick = function() {
			table.deleteRow(row_number-1);
		};
		cell3.appendChild(delete_btn);
	});
  }
  
  function getTableData() {
    var table = document.getElementById("mtf_table_form");
    var row;
	var params = {};
    for (var i = 1; i < table.rows.length ;i++) {
        console.log('here')
        var x =  table.rows[i].cells[0].childNodes[0].value;
        var y =  table.rows[i].cells[1].childNodes[0].value;
		console.log(x,y)
		params[x] = Number(y);
    }
	console.log(params);
	return params;
  }
  
  $('#create_start').click(function(){ addStartWidget();});
  $('#create_init_camera').click(function(){ addOperationWidget("Init Camera"); });
  $('#create_y_level').click(function(){ addOperationWidget("Y_Level"); });
  $('#create_load_camera').click(function () { addOperationWidget("Load Camera"); });
  $('#create_pr_to_bond').click(function () { addOperationWidget("PR To Bond"); });
  $('#create_initial_tilt').click(function () { addOperationWidget("Initial Tilt"); });
  $('#create_unload_camera').click(function () { addMultipleOperationWidget("Unload Camera"); });
  $('#create_aa').click(function () { addOperationWidget("AA"); });
  $('#create_oc').click(function () { addMultipleOperationWidget("OC"); });
  $('#create_mtf').click(function () { addMultipleOperationWidget("MTF"); });
  $('#create_z_offset').click(function () { addOperationWidget("Z Offset"); });
  $('#create_xy_offset').click(function () { addOperationWidget("XY Offset"); });
  $('#create_save_image').click(function () { addOperationWidget("Save Image"); });
  $('#create_disp').click(function () { addMultipleOperationWidget("Dispense"); });
  $('#create_uv').click(function () { addMultipleOperationWidget("UV"); });
  $('#create_init_lens').click(function () { addMultipleOperationWidget("Init_Lens"); });
  $('#create_delay').click(function () { addOperationWidget("Delay"); });
  $('#create_accept').click(function () { addEndWidget("Accept"); });
  $('#create_reject').click(function () { addEndWidget("Reject"); });
  $('#create_terminate').click(function () { addEndWidget("Terminate"); });
  $('#create_thread').click(function () { addThreadWidget("Parallel"); });
  $('#create_join_thread').click(function () { addJoinThreadWidget("Join"); });
  $('#create_load_material').click(function () { addOperationWidget("Load Material"); });
  $('#create_otp').click(function () { addOperationWidget("OTP"); });
  $('#create_grr').click(function () { addOperationWidget("GRR"); });
  $('#create_partical_check').click(function () { addOperationWidget("Partical Check"); });


  $('#get_data').click(function () {
    var data = $flowchart.flowchart('getData');
    $('#flowchart_data').val(JSON.stringify(data, null, 2));
  });
  
  
  $('#set_data').click(function () {
    var data = JSON.parse($('#flowchart_data').val());
    $flowchart.flowchart('setData', data);
  });
  
  $('#aa_select_mode').click(function() {
	 console.log("Selecting aa mode");
	 document.getElementById("myDropdown").classList.toggle("show");
  });
  
  $('#select_zscan_mode').click(function(){
	  console.log("Selecting aa zscan mode");
	  $('#aa_mode').val(0);
	  $('#aa1_start_position_div')[0].style.display = "block";
	  $('#aa1_stop_position_div')[0].style.display = "block";
	  $('#aa2_start_position_div')[0].style.display = "block";
	  $('#aa2_stop_position_div')[0].style.display = "block";
	  $('#aa_offset_in_um_div')[0].style.display = "none";
	  
  });
  
  $('#select_dfov_mode').click(function(){
	  console.log("Selecting aa dfov mode");
	  $('#aa_mode').val(1);
	  $('#aa1_start_position_div')[0].style.display = "block";
	  $('#aa1_stop_position_div')[0].style.display = "block";
	  $('#aa2_start_position_div')[0].style.display = "block";
	  $('#aa2_stop_position_div')[0].style.display = "block";
	  $('#aa_offset_in_um_div')[0].style.display = "block";
  });
  
  $('#select_stationary_scan_mode').click(function(){
	  console.log("Selecting aa stationary scan mode");
	  $('#aa_mode').val(2);
	  $('#aa1_start_position_div')[0].style.display = "none";
	  $('#aa1_stop_position_div')[0].style.display = "none";
	  $('#aa2_start_position_div')[0].style.display = "none";
	  $('#aa2_stop_position_div')[0].style.display = "none";
	  $('#aa_offset_in_um_div')[0].style.display = "block";
  });
  
   $('#select_x_scan_mode').click(function(){
	  console.log("Selecting aa stationary scan mode");
	  $('#aa_mode').val(3);
  });
  
  $flowchart.siblings('.delete_selected_button').click(function () { $flowchart.flowchart('deleteSelected'); });
  
  $('#update_data_button').click(function(){
       var selectedOperatorId = $flowchart.flowchart('getSelectedOperatorId');
    if (selectedOperatorId == null) { return; }
    console.log('update opeator: ' + selectedOperatorId);
    if (selectedOperatorId.includes("AA_")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $aa_operatorTitle.val());
      var params = {
        mode: Number($('#aa_mode').val()),
		aa1_start_pos: Number($('#aa1_start_position').val()),
		aa1_stop_pos: Number($('#aa1_stop_position').val()),
		aa2_start_pos: Number($('#aa2_start_position').val()),
		aa2_stop_pos: Number($('#aa2_stop_position').val()),
        step_size: Number($('#aa_step_size').val()), 
		offset_in_um: Number($('#aa_offset_in_um').val()),
		delay_Z_in_ms: Number($('#aa_delay_Z_in_ms').val()),
        image_count: Number($('#aa_image_count').val()),
        enable_tilt: Number($('#aa_enable_tilt').val()),
        position_checking: Number($('#aa_position_checking').val()), 
		edge_filter: Number($('#aa_edge_filter').val()), 
		is_debug: Number($('#aa_is_debug').val()),
		delay_in_ms: Number($('#aa_delay_in_ms').val())
      };

      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("Initial Tilt")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $initial_tilt_operatorTitle.val());
      var params = { roll: Number($('#roll').val()),
	  pitch: Number($('#pitch').val()),
	  delay_in_ms:Number($('#initial_tilt_delay_in_ms'))
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("OC")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $oc_operatorTitle.val());
      var params = {
        enable_motion: Number($('#oc_enable_motion').val()),
        fast_mode: Number($('#oc_fast_mode').val()),
		mode: Number($('#oc_mode').val()),
		is_debug: Number($('#oc_is_debug').val()),
        delay_in_ms: Number($('#oc_delay_in_ms').val()),
		retry: Number($('#oc_retry').val()),
		is_check: Number($('#oc_is_check').val()),
		x_limit_in_um: Number($('#oc_x_limit_in_um').val()),
		y_limit_in_um: Number($('#oc_y_limit_in_um').val()),
		oc_intensity_threshold: Number($('#oc_intensity_threshold').val())
      };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("Z Offset")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $aa_z_offset_operator_title.val());
      var params = { type: Number($('#aa_z_offset_type').val()),
	  z_offset_in_um_aa1: Number($('#z_offset_in_um_aa1').val()),
	  z_offset_in_um_aa2: Number($('#z_offset_in_um_aa2').val()),
	  delay_in_ms: Number($('#aa_z_offset_delay_in_ms').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("XY Offset")) {
      $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $aa_xy_offset_operator_title.val());
      var params = { type: Number($('#aa_xy_offset_type').val()),
	  x_offset_in_um_aa1: Number($('#aa_x_offset_in_um_aa1').val()),
	  y_offset_in_um_aa1: Number($('#aa_y_offset_in_um_aa1').val()),
	  x_offset_in_um_aa2: Number($('#aa_x_offset_in_um_aa2').val()),
	  y_offset_in_um_aa2: Number($('#aa_y_offset_in_um_aa2').val()),
	  delay_in_ms: Number($('#aa_xy_offset_delay_in_ms').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("Save Image")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $save_image_operator_title.val());
      var params = { type: Number($('#save_image_type').val()),
	  lighting: $('#save_image_lighting').val()
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("Dispense")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $dispense_operator_title.val());
      var params = {x_offset_in_um:  Number($('#dispense_x_offset_in_um').val()),
	  y_offset_in_um:  Number($('#dispense_y_offset_in_um').val()),
	  z_offset_in_um:  Number($('#dispense_z_offset_in_um').val()),
	  enable_glue_inspection: Number($('#enable_glue_inspection').val()),
	  glue_inspection_mode: Number($('#glue_inspection_mode').val()),
	  min_glue_width_in_mm: Number($('#min_glue_width_in_mm').val()),
	  max_glue_width_in_mm: Number($('#max_glue_width_in_mm').val()),
	  max_avg_glue_width_in_mm: Number($('#max_avg_glue_width_in_mm').val()),
	  delay_in_ms:  Number($('#dispense_delay_in_ms').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("MTF")) {
	  getTableData();
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#mtf_operator_title').val());
      var params = getTableData();
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("GRR")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#grr_operator_title').val());
      var params = { change_lens: Number($('#grr_change_lens').val()),
	  change_sensor: Number($('#grr_change_sensor').val()),
	  repeat_time:  Number($('#grr_repeat_time').val()), 
	  change_time:  Number($('#grr_change_time').val())};
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("Y_Level")){
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#y_level_operator_title').val());
	  var params = { enable_plot: Number($('#y_level_enable_plot').val()),
	  min: Number($('#y_level_min').val()),
	  max: Number($('#y_level_max').val()),
	  mode: Number($('#y_level_mode').val()),
	  margin: Number($('#y_level_margin').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("UV")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#uv_operator_title').val());
	  var params = { time_in_ms: Number($('#uv_time_in_ms').val()),
	  enable_OTP:  Number($('#uv_enable_OTP').val()),
	  enable_y_level_check: Number($('#uv_enable_y_level_check').val()),
	  y_level_path: Number($('#uv_y_level_path').val()),
	  delay_before_check: Number($('#uv_delay_before_check').val()),
	  margin: Number($('#uv_y_level_margin').val()),
	  min: Number($('#uv_y_level_min_spec').val()),
	  max: Number($('#uv_y_level_max_spec').val()),
	  change_allowance: Number($('#uv_y_level_change_allowance').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else if (selectedOperatorId.includes("Init_Lens")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#init_lens_operator_title').val());
	  var params = { delay_in_ms: Number($('#init_lens_delay_in_ms').val()),
	  cmd: Number($('#init_lens_cmd').val()),
	  ois_x_target_position:  Number($('#init_lens_ois_x_target_position').val()),
	  ois_y_target_position:  Number($('#init_lens_ois_y_target_position').val()),
	  target_position:  Number($('#init_lens_target_position').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
	} else if (selectedOperatorId.includes("Partical Check")) {
	  $flowchart.flowchart('setOperatorTitle', selectedOperatorId, $('#partical_check_operator_title').val());
	  var params = { delay_in_ms: Number($('#partical_check_delay_in_ms').val()),
	  enable_partical_check: Number($('#enable_partical_check').val()),
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    } else {
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, init_basic_params);
      var params = { retry: Number($('#basic_retry').val()),
	  delay_in_ms: Number($('#basic_delay').val())
	  };
      $flowchart.flowchart('setOperatorParams', selectedOperatorId, params);
    }
    alert(selectedOperatorId + " Params is updated");
  });
  
  var toggle_edit_state = false;
  $('#close_modal_button').click(function(){
	toggle_edit_state = false;
	$("a[rel='modal:close']").click();
  });
  window.onkeyup = function(event) {
    let key = event.key.toUpperCase();
	var selectedOperatorId = $flowchart.flowchart('getSelectedOperatorId');
	console.log("Key up event: " + key + " selected OperatorId: " + selectedOperatorId);
    if ( key == 'W' ) {
        // 'W' key is pressed
		updateData();
    } else if ( key == 'E' && !toggle_edit_state) {
		if ($('#aa_mode').val() == "0"){
			$('#aa1_start_position_div')[0].style.display = "block";
			$('#aa1_stop_position_div')[0].style.display = "block";
			$('#aa2_start_position_div')[0].style.display = "block";
			$('#aa2_stop_position_div')[0].style.display = "block";
			$('#aa_offset_in_um_div')[0].style.display = "none";
		} else if (($('#aa_mode').val() == "1")) {
			$('#aa1_start_position_div')[0].style.display = "block";
			$('#aa1_stop_position_div')[0].style.display = "block";
			$('#aa2_start_position_div')[0].style.display = "block";
			$('#aa2_stop_position_div')[0].style.display = "block";
			$('#aa_offset_in_um_div')[0].style.display = "block";
		} else if (($('#aa_mode').val() == "2")) {
			$('#aa1_start_position_div')[0].style.display = "none";
			$('#aa1_stop_position_div')[0].style.display = "none";
			$('#aa2_start_position_div')[0].style.display = "none";
			$('#aa2_stop_position_div')[0].style.display = "none";
			$('#aa_offset_in_um_div')[0].style.display = "block";
		}
		$('#ex1').modal({
			showClose: false,
			escapeClose: false,
			clickClose: false
		});
		toggle_edit_state = true;
    } else if (key == 'DELETE') {
		$flowchart.flowchart('deleteSelected');
	} else if (key == 'C' && !toggle_edit_state) {
		duplicateOperationWidget();
	} else if (key == 'ESCAPE') {
		toggle_edit_state = false;
		$("a[rel='modal:close']").click();
	}
  }
  window.onclick = function(event) {
	if (!event.target.matches('.dropbtn')) {
		var dropdowns = document.getElementsByClassName("dropdown-content");
		var i;
		for (i = 0; i < dropdowns.length; i++) {
			var openDropdown = dropdowns[i];
			if (openDropdown.classList.contains('show')) {
				openDropdown.classList.remove('show');
			}
		}
	}
  }
  
});