#ifndef __PIPE_STAGE__
#define __PIPE_STAGE__

#include "sram.hpp"

static long long global_serial_num = 0;

struct PipeOp {
    PipeOp(mem_addr nbin_addr, mem_size nbin_size,
        mem_addr sb_addr, mem_size sb_size,
        mem_addr nbout_addr, mem_size nbout_size);
    ~PipeOp() {};

    bool data_is_ready();
    bool write_is_complete();
    
    long long serial_num;
    SramOp nbin_sram_op;
    SramOp sb_sram_op;
    SramOp nbout_sram_op;
    bool is_pending;
};

typedef std::deque<PipeOp *> PipeOpReg;

class PipeStage {
public:
    PipeStage(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    virtual ~PipeStage();

    virtual void do_op() = 0;
    virtual void tick();
    virtual void print();

protected:
    bool is_pipe_op_reg_empty(PipeOpReg *reg);
    bool is_pipe_op_reg_full(PipeOpReg *reg);
    
    PipeOpReg *reg_in;
    PipeOpReg *reg_out;
    int queue_size;
    int n_stages;

    std::string name;
    
    PipeOp **pipeline;
};

#endif
