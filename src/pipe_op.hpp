#ifndef __PIPE_OP__
#define __PIPE_OP__

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

typedef std::deque<PipeOp *> PipeOpReg;

#endif
