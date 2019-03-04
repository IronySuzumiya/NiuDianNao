#ifndef __CONTROL_PROCESSOR__
#define __CONTROL_PROCESSOR__

#include "dram.hh"
#include "config.hh"
#include "datapath.hh"

struct ControlInstruction {
    enum ci_state {
      LOAD_NBIN = 0,
      LOAD_SB,
      DO_OP,
      STORE_NBOUT
    };
    cp_inst_state   m_state;

    enum ci_op {
      NOP,
      LOAD,
      STORE,
      READ,
      WRITE,
      MULT,
      ADD,
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
};

typedef queue<ControlInstruction> CIQueue;

class ControlProcessor {
public:
    ControlProcessor(DnnConfig *cfg, Datapath *dp);
    ~ControlProcessor();

    void tick();
    bool read_instructions(std::istream& is);
    bool read_instructions(CIQueue *ciq);

    void read_complete_callback(unsigned id, uint64_t address, uint64_t clock_cycle);
    void write_complete_callback(unsigned id, uint64_t address, uint64_t clock_cycle);
    
private:
    bool execute_instruction(ControlInstruction *ci);

    bool read_sram(Sram *sram, mem_addr addr, mem_size size);
    bool write_sram(Sram *sram, mem_addr addr, mem_size size);

    bool read_dram(Dram mem_addr addr, mem_size size);
    bool write_dram(mem_addr addr, mem_size size);
    
    DRAMSim::TransactionCompleteCB *read_callback;
    DRAMSim::TransactionCompleteCB *write_callback;

    DnnConfig *cfg;
    datapath *dp;
    CIQueue ciq;

    int sb_index;   // used to track progress of instruction in the DO_OP state
                    // since SB entries are never reused
};

#endif
