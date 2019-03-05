#ifndef __CONTROL_PROCESSOR__
#define __CONTROL_PROCESSOR__

#include "dram.hh"
#include "config.hh"
#include "datapath.hh"
#include "control_instruction.hh"

class ControlProcessor {
public:
    ControlProcessor(DnnConfig *cfg, Datapath *dp);
    ~ControlProcessor() {};

    void tick();
    bool read_instructions(std::ifstream& is);
    bool read_instructions(std::istringstream& is);
    
private:
    bool execute_instruction(ControlInstruction *ci);

    DnnConfig *cfg;
    Datapath *dp;
    CIQueue ciq;

    std::queue<LoadStoreOp> mem_ops;

    bool nbout_store;

    int sb_index;   // used to track progress of instruction in the DO_OP state
                    // since SB entries are never reused
};

#endif
