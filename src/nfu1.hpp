#ifndef __NFU1__
#define __NFU1__

#include "pipe_stage.hpp"

#include "sram.hpp"

class PipeStageNFU1 : public PipeStage {
public:

    PipeStageNFU1(PipeOpReg *reg_in, PipeOpReg *reg_out, PipeOpReg *reg_requests, Sram * nbin, Sram * sb, 
    int queue_size, int n_stages, unsigned num_multipliers);

    PipeStageNFU1(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_multipliers);
    ~PipeStageNFU1();

    void tick();
    void print();

private:
    // Functional units
    PipeOpReg* m_requests;

    Sram * nbin;
    Sram * sb;

    unsigned m_num_multipliers;
    functional_unit **m_multipliers;    // Multipliers


};

#endif
