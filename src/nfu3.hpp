#ifndef __NFU3__
#define __NFU3__

#include "pipe_stage.hpp"
#include "functional_unit.hpp"

class PipeStageNFU3 : public PipeStage {
public:
    PipeStageNFU3(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_multipliers, unsigned num_adders, Sram *nbout);
    ~PipeStageNFU3();

    void activate();
    void deactivate();

    void tick();
    void print();

private:
    void do_op();
    bool is_ready_to_fetch(PipeOp *op);
    void preprocess_op(PipeOp *op);

    void write_nbout(SramOp *op);

    unsigned num_multipliers;
    unsigned num_adders;
    FunctionalUnit **multipliers;
    FunctionalUnit **adders;

    bool is_activated;

    Sram *nbout;
};

#endif
