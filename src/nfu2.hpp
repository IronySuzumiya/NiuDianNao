#ifndef __NFU2__
#define __NFU2__

#include "pipe_stage.hpp"
#include "functional_unit.hpp"

class PipeStageNFU2 : public PipeStage {
public:
    PipeStageNFU2(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages, unsigned num_adders, unsigned num_shifters, unsigned num_max, Sram *nbout);
    ~PipeStageNFU2();

    void to_add_mode();
    void to_max_mode();

    void in_reset();
    void in_from_nbout();

    void tick();
private:
    void do_op();
    bool is_ready_to_fetch(PipeOp *op);
    void preprocess_op(PipeOp *op);

    void read_nbout(SramOp *op);
    void write_nbout(SramOp *op);

    unsigned num_adders;
    unsigned num_shifters;
    unsigned num_max;
    FunctionalUnit **adders;
    FunctionalUnit **shifters;
    FunctionalUnit **max;

    bool is_in_add_mode;
    bool is_from_nbout;

    Sram *nbout;
};

#endif