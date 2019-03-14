#include "config.hpp"

void DnnConfig::reg_options(option_parser_t opp){
    // Floating point bit width precision
    option_parser_register(opp, "-bit_width", OPT_INT32, &bit_width,
                           "Floating point bit width (default = 32)",
                           "32");
    
    // Pipeline stage buffer size
    option_parser_register(opp, "-max_buffer_size", OPT_INT32, &max_buffer_size,
                           "Maximum buffer queue size between pipeline stages (default = 8)",
                           "8");
    
    // SB
    option_parser_register(opp, "-sb_line_length", OPT_INT32, &sb_line_length,
                           "SB line length (default = 256)",
                           "256");
    
    option_parser_register(opp, "-sb_num_lines", OPT_INT32, &sb_num_lines,
                           "Number of SB lines (default = 64)",
                           "64");
    
    option_parser_register(opp, "-sb_access_cycles", OPT_INT32, &sb_access_cycles,
                           "Number of cycles to access the SB SRAM (default = 1)",
                           "1");
    
    option_parser_register(opp, "-sb_num_ports", OPT_INT32, &sb_num_ports,
                           "Number of SRAM Read/Write ports (default = 1)",
                           "1");
    
    // NBin
    option_parser_register(opp, "-nbin_line_length", OPT_INT32, &nbin_line_length,
                           "SB line length (default = 256)",
                           "256");
    
    option_parser_register(opp, "-nbin_num_lines", OPT_INT32, &nbin_num_lines,
                           "Number of NBin lines (default = 64)",
                           "64");
    
    option_parser_register(opp, "-nbin_access_cycles", OPT_INT32, &nbin_access_cycles,
                           "Number of cycles to access the NBin SRAM (default = 1)",
                           "1");
    
    option_parser_register(opp, "-nbin_num_ports", OPT_INT32, &nbin_num_ports,
                           "Number of SRAM Read/Write ports (default = 1)",
                           "1");
    
    // NBout
    option_parser_register(opp, "-nbout_line_length", OPT_INT32, &nbout_line_length,
                           "NBout line length (default = 256)",
                           "256");
    
    option_parser_register(opp, "-nbout_num_lines", OPT_INT32, &nbout_num_lines,
                           "Number of NBout lines (default = 64)",
                           "64");
    
    option_parser_register(opp, "-nbout_access_cycles", OPT_INT32, &nbout_access_cycles,
                           "Number of cycles to access the NBout SRAM (default = 1)",
                           "1");
    
    option_parser_register(opp, "-nbout_num_ports", OPT_INT32, &nbout_num_ports,
                           "Number of SRAM Read/Write ports (default = 1)",
                           "1");
    
    // NFU-1
    option_parser_register(opp, "-num_nfu1_pipeline_stages", OPT_INT32, &num_nfu1_pipeline_stages,
                           "Number of NFU-1 internal pipeline stages (default = 1)",
                           "1");
    option_parser_register(opp, "-num_nfu1_multipliers", OPT_INT32, &num_nfu1_multipliers,
                           "Number of NFU-1 multipliers (default = 256)",
                           "256");
    // NFU-2
    option_parser_register(opp, "-num_nfu2_pipeline_stages", OPT_INT32, &num_nfu2_pipeline_stages,
                           "Number of NFU-2 internal pipeline stages (default = 1)",
                           "1");
    option_parser_register(opp, "-num_nfu2_adders", OPT_INT32, &num_nfu2_adders,
                           "Number of NFU-2 adders (default = 16 * 15)",
                           "240");
    option_parser_register(opp, "-num_nfu2_shifters", OPT_INT32, &num_nfu2_shifters,
                           "Number of NFU-2 shifters (default = 1)",
                           "1");
    option_parser_register(opp, "-num_nfu2_max", OPT_INT32, &num_nfu2_max,
                           "Number of NFU-2 max (default = 1)",
                           "1");
    // NFU-3
    option_parser_register(opp, "-num_nfu3_pipeline_stages", OPT_INT32, &num_nfu3_pipeline_stages,
                           "Number of NFU-3 internal pipeline stages (default = 1)",
                           "1");
    option_parser_register(opp, "-num_nfu3_multipliers", OPT_INT32, &num_nfu3_multipliers,
                           "Number of NFU-3 multipliers (default = 16)",
                           "16");
    option_parser_register(opp, "-num_nfu3_adders", OPT_INT32, &num_nfu3_adders,
                           "Number of NFU-3 adders (default = 16)",
                           "16");
    
    option_parser_register(opp, "-num_outputs", OPT_INT32, &num_outputs,
                           "Number of output feature maps (default = 256)",
                           "256");

    option_parser_register(opp, "-sb_acc_eng", OPT_FLOAT, &sb_acc_eng,
                           "SB access energy (default = 0)",
                           "0");

    option_parser_register(opp, "-nbin_acc_eng", OPT_FLOAT, &nbin_acc_eng,
                           "NBin access energy (default = 0)",
                           "0");

    option_parser_register(opp, "-nbout_acc_eng", OPT_FLOAT, &nbout_acc_eng,
                           "NBout access energy (default = 0)",
                           "0");

    option_parser_register(opp, "-int_mult_16_eng", OPT_FLOAT, &int_mult_16_eng,
                           "16-bit integer multiply energy (default = 0)",
                           "0");

    option_parser_register(opp, "-fp_mult_16_eng", OPT_FLOAT, &fp_mult_16_eng,
                           "16-bit floating-point multiply energy (default = 0)",
                           "0");

    option_parser_register(opp, "-int_add_16_eng", OPT_FLOAT, &int_add_16_eng,
                           "16-bit integer add energy (default = 0)",
                           "0");

    option_parser_register(opp, "-fp_add_16_eng", OPT_FLOAT, &fp_add_16_eng,
                           "16-bit floating-point add energy (default = 0)",
                           "0");
}
