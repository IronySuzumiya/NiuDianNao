#ifndef __DNN_CONFIG__
#define __DNN_CONFIG__

#include "option_parser.h"

class DnnConfig {
public:
    DnnConfig() {};
    ~DnnConfig() {};
    
    void reg_options(option_parser_t opp);
    
    // Bit-width
    unsigned bit_width;
    
    // Pipeline stage buffer size
    unsigned max_buffer_size;
    
    // SB
    unsigned sb_line_length;
    unsigned sb_num_lines;
    unsigned sb_access_cycles;
    unsigned sb_num_ports;

    // NBin
    unsigned nbin_line_length;
    unsigned nbin_num_lines;
    unsigned nbin_access_cycles;
    unsigned nbin_num_ports;
    
    // NBout
    unsigned nbout_line_length;
    unsigned nbout_num_lines;
    unsigned nbout_access_cycles;
    unsigned nbout_num_ports;
    
    // NFU-1
    unsigned num_nfu1_pipeline_stages;
    unsigned num_nfu1_multipliers;

    // NFU-2
    unsigned num_nfu2_pipeline_stages;
    unsigned num_nfu2_adders;
    unsigned num_nfu2_shifters;
    unsigned num_nfu2_max;
    
    // NFU-3    
    unsigned num_nfu3_pipeline_stages;
    unsigned num_nfu3_multipliers;
    unsigned num_nfu3_adders;

    // N_o need to change for each layer
    // this probably shouldn't be a parameter but I don't know how you can infer it from the instructions
    unsigned num_outputs;

    // Simple power model (factor in pJ)
    // SRAM Arrays
    float sb_acc_eng;
    float nbin_acc_eng;
    float nbout_acc_eng;

    // Functional units
    float int_mult_16_eng;
    float fp_mult_16_eng;
    float int_add_16_eng;
    float fp_add_16_eng;
};

#endif
