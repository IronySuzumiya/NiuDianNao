#ifndef __NFU1__
#define __NFU1__

#include "pipe_stage.hpp"
#include "functional_unit.hpp"

class PipeStageNFU1 : public PipeStage {
public:
    PipeStageNFU1(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_multipliers);
    ~PipeStageNFU1();

    void do_op();

private:
    unsigned num_multipliers;
    FunctionalUnit **multipliers;
};

#endif
