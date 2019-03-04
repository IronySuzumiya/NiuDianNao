#ifndef __CONTROL_PROCESSOR__
#define __CONTROL_PROCESSOR__

#include "dram.hh"
#include "config.hh"
#include "datapath.hh"
#include "control_instruction.hh"

typedef struct dram_op {
    mem_addr addr;
    mem_size size;
    bool is_read;
    bool is_complete;
} DramOp;

typedef queue<DramOp> DramOpReg;

class ControlProcessor {
public:
    ControlProcessor(DnnConfig *cfg, Datapath *dp, Dram *dram);
    ~ControlProcessor() {};

    void tick();
    bool read_instructions(std::ifstream& is);
    bool read_instructions(std::istringstream& is);

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
    Datapath *dp;
    Dram *dram;
    CIQueue ciq;
    DramOpReg requests;
    PipeOpReg 

    int sb_index;   // used to track progress of instruction in the DO_OP state
                    // since SB entries are never reused
};

#endif
