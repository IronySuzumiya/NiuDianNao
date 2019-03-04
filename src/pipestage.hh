#ifndef __PIPE_STAGE__
#define __PIPE_STAGE__

#include "pipeoperation.hh"

class PipeStage {
public:
    PipeStage(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    virtual ~PipeStage();

    bool push_op(PipeOp *op);

    virtual void cycle() = 0;
    virtual void print();

protected:
    bool is_pipe_op_reg_empty(PipeOpReg *reg);
    bool is_pipe_op_reg_full(PipeOpReg *reg);
    
    PipeOpReg *reg_in;
    PipeOpReg *reg_out;
    int queue_size;
    int n_stages;
    
    PipeOp **pipeline;
};

#endif
