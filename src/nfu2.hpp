#ifndef __NFU2__
#define __NFU2__

#include "pipe_stage.hpp"
#include"functional_unit.hpp"


class PipeStageNFU2 : public PipeStage {
public:

    PipeStageNFU2(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_adders, unsigned num_shifters, unsigned num_max);
    ~PipeStageNFU2();

    void tick();
    void print();


private:
    // Functional units
    unsigned m_num_adders;
    unsigned m_num_shifters;
    unsigned m_num_max;
    
    functional_unit **m_adders;     // Adder trees
    functional_unit **m_shifters;   // Shifter
    functional_unit **m_max;        // Max operator
};

#endif