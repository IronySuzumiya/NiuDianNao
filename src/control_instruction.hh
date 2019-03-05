#ifndef __CONTROL_INSTRUCTION__
#define __CONTROL_INSTRUCTION__

#include "common.hh"
#include "dram.hh"

typedef struct control_instruction {
    enum ci_state {
        BEGIN,
        LOAD_SB,
        LOAD_NBIN,
        DO_OP,
        STORE_NBOUT
    };
    ci_state   state;

    enum ci_op {
        NOP,
        LOAD,
        STORE,
        READ,
        WRITE,
        MULT,
        ADD,
        MAX,
        RESET,
        NBOUT,
        NFU3,
        SIGMOID,
        INVALID
    };

    ci_op   cp_end; 
    ci_op   sb_read_op;
    int     sb_reuse;
    int     sb_address;
    int     sb_size;
    ci_op   nbin_read_op;
    int     nbin_reuse;
    int     nbin_stride;
    int     nbin_stride_begin;
    int     nbin_stride_end;
    int     nbin_address;
    int     nbin_size;
    ci_op   nbout_read_op;
    ci_op   nbout_write_op;
    int     nbout_address;
    int     nbout_size;
    ci_op   nfu_nfu1_op;
    ci_op   nfu_nfu2_op;
    ci_op   nfu_nfu2_in;
    ci_op   nfu_nfu2_out;
    ci_op   nfu_nfu3_op;
    int     nfu_output_begin;
    int     nfu_output_end;

    DramOp  dram_op;
} ControlInstruction;

typedef queue<ControlInstruction> CIQueue;

#endif
