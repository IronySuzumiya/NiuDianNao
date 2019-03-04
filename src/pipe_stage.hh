#ifndef __PIPE_STAGE__
#define __PIPE_STAGE__

#include "sram.hh"

class PipeOp {
public:
    int serial_num;
    SramOp nbin_sram_op;
    SramOp sb_sram_op;
    SramOp nbout_sram_op;
    bool is_read;
    bool data_is_ready;
};

typedef std::queue<PipeOp *> PipeOpReg;

class PipeStage {
public:
    PipeStage(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    virtual ~PipeStage();

    bool push_op(PipeOp *op);

    virtual void tick() = 0;
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
