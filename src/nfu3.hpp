#ifndef __NFU3__
#define __NFU3__

#include "pipe_stage.hpp"

#include "config.hpp"
#include "functional_unit.h"

class PipeStageNFU3 : public PipeStage {
public:

    PipeStageNFU3(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_multipliers, unsigned num_adders);
    ~PipeStageNFU3();

    void tick();
    void print();

private:
    // Functional units
    unsigned m_num_multipliers;
    unsigned m_num_adders;
    functional_unit **m_multipliers;    // Multipliers
    functional_unit **m_adders;         // Adders
};

#endif
