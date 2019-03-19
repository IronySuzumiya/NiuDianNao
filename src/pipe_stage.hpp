#ifndef __PIPE_STAGE__
#define __PIPE_STAGE__

#include "pipe_op.hpp"

class PipeStage {
public:
    PipeStage(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    virtual ~PipeStage();

    virtual void tick();
    virtual void print();

    bool is_pipe_line_empty();

protected:
    virtual void do_op() = 0;
    virtual bool is_ready_to_fetch(PipeOp *op) = 0;
    virtual void preprocess_op(PipeOp *op) = 0;

    bool is_pipe_op_reg_empty(PipeOpReg *reg);
    bool is_pipe_op_reg_full(PipeOpReg *reg);

    void print_reg_out_as_nbout_write();

    PipeOpReg *reg_in;
    PipeOpReg *reg_out;
    int queue_size;
    int n_stages;

    std::string name;
    
    PipeOp **pipeline;
};

#endif
