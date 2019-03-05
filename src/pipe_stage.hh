#ifndef __PIPE_STAGE__
#define __PIPE_STAGE__

#include "sram.hh"

static int64_t global_serial_num = 0;

class PipeOp {
public:
    PipeOp(mem_addr nbin_addr, mem_size nbin_size,
        mem_addr sb_addr, mem_size sb_size,
        mem_addr nbout_addr, mem_size nbout_size);
    ~PipeOp() {};

    bool data_is_ready();

private:
    int serial_num;
    SramOp nbin_sram_op;
    SramOp sb_sram_op;
    SramOp nbout_sram_op;
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
