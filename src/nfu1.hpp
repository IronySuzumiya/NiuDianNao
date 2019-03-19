#ifndef __NFU1__
#define __NFU1__

#include "pipe_stage.hpp"
#include "functional_unit.hpp"

class PipeStageNFU1 : public PipeStage {
public:
    PipeStageNFU1(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_multipliers, Sram *nbin, Sram *sb);
    ~PipeStageNFU1();

private:
    void do_op();
    bool is_ready_to_fetch(PipeOp *op);
    void preprocess_op(PipeOp *op);

    void read_nbin(SramOp *op);
    void read_sb(SramOp *op);

    unsigned num_multipliers;
    FunctionalUnit **multipliers;

    Sram *nbin;
    Sram *sb;
};

#endif
