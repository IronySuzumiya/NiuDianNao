#ifndef __PIPE_STAGE__
#define __PIPE_STAGE__

#include "sram.hpp"

static long long global_serial_num = 0;

struct PipeOp {
    PipeOp(mem_addr nbin_addr, mem_size nbin_size,
        mem_addr sb_addr, mem_size sb_size,
        mem_addr nbout_addr, mem_size nbout_size);
    ~PipeOp() {};

    bool is_ready_to_nfu1();
    bool is_ready_to_nfu2();
    bool is_complete();
    
    long long serial_num;
    SramOp nbin_read_op;
    SramOp sb_read_op;
    SramOp nbout_read_op;
    SramOp nbout_write_op;
    bool is_pending;
};

struct PipeOpReg;

class PipeStage {
public:
    PipeStage(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    virtual ~PipeStage();

    virtual void tick();
    virtual void print();

protected:
    virtual void do_op() = 0;
    virtual bool is_ready_to_fetch(PipeOp *op) = 0;
    virtual void read_data(PipeOp *op) = 0;

    bool is_pipe_op_reg_empty(PipeOpReg *reg);
    bool is_pipe_op_reg_full(PipeOpReg *reg);

    PipeOpReg *reg_in;
    PipeOpReg *reg_out;
    int queue_size;
    int n_stages;

    bool needs_data;

    std::string name;
    
    PipeOp **pipeline;
};

#endif
