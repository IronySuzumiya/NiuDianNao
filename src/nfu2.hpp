#ifndef __NFU2__
#define __NFU2__

#include "pipe_stage.hpp"
#include "functional_unit.hpp"

class PipeStageNFU2 : public PipeStage {
public:
    PipeStageNFU2(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_adders, unsigned num_shifters, unsigned num_max);
    ~PipeStageNFU2();

    void do_op();

private:
    unsigned num_adders;
    unsigned num_shifters;
    unsigned num_max;
    FunctionalUnit **adders;
    FunctionalUnit **shifters;
    FunctionalUnit **max;
};

#endif