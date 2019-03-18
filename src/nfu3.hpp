#ifndef __NFU3__
#define __NFU3__

#include "pipe_stage.hpp"
#include "functional_unit.hpp"

class PipeStageNFU3 : public PipeStage {
public:
    PipeStageNFU3(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_multipliers, unsigned num_adders);
    ~PipeStageNFU3();

private:
    void do_op();
    bool is_ready_to_fetch(PipeOp *op);
    void read_data(PipeOp *op);

    unsigned num_multipliers;
    unsigned num_adders;
    FunctionalUnit **multipliers;
    FunctionalUnit **adders;
};

#endif
