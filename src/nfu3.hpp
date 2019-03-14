#ifndef __NFU3__
#define __NFU3__

#include "pipe_stage.hpp"
#include "functional_unit.hpp"

class PipeStageNFU3 : public PipeStage {
public:
    PipeStageNFU3(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_multipliers, unsigned num_adders);
    ~PipeStageNFU3();

    void do_op();

private:
    unsigned num_multipliers;
    unsigned num_adders;
    FunctionalUnit **multipliers;
    FunctionalUnit **adders;
};

#endif
