#ifndef __CONTROL_PROCESSOR__
#define __CONTROL_PROCESSOR__

#include "datapath.hpp"
#include "control_instruction.hpp"

class ControlProcessor {
public:
    ControlProcessor(DnnConfig *cfg, Datapath *dp);
    ~ControlProcessor() {};

    void tick();
    void read_instructions(std::string s);
    void read_instructions(std::istream& is);

    bool is_working();
    
private:
    bool execute_instruction(ControlInstruction *ci);

    DnnConfig *cfg;
    Datapath *dp;
    CIQueue ciq;

    int num_output_lines;
    int data_size;
    int nfu3_flag;
    bool nfu3_on;
    bool nbout_store;

    int sb_index;   // used to track progress of instruction in the DO_OP state
                    // since SB entries are never reused
};

#endif
