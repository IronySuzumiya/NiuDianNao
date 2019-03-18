#ifndef __DATA_PATH__
#define __DATA_PATH__

#include "dram.hpp"
#include "sram.hpp"
#include "nfu1.hpp"
#include "nfu2.hpp"
#include "nfu3.hpp"
#include "config.hpp"

struct LoadStoreOp {
    Sram *sram;
    DramOp dram_op;
    SramOp sram_op;
    bool is_sent;
};

struct PipeOpReg {
    // TODO: this struct holds a deque to store PipeOps, 
    //       should provide interfaces for pushing PipeOps in,
    //       poping PipeOps out, checking the size of the deque,
    //       and also reading data from SRAM corresponding to
    //       the information held by the PipeOp.
};

class Datapath {
    enum datapath_mode {
        nfu2_to_nbout,
        nfu3_to_nbout
    };

public:
    Datapath(DnnConfig *cfg);
    
    ~Datapath();

    void tick();

    void print_stats();
    void print_pipeline();

    bool is_ready();

    void push_pipe_op(PipeOp *op);

    void load_nbin(mem_addr dram_addr, mem_addr sram_addr, mem_size size);
    void load_sb(mem_addr dram_addr, mem_addr sram_addr, mem_size size);
    void store_nbout(mem_addr dram_addr, mem_addr sram_addr, mem_size size);

    void read_nbin(SramOp *op);
    void read_sb(SramOp *op);
    void read_nbout(SramOp *op);
    void write_nbout(SramOp *op);

private:
    DnnConfig *cfg;
    
    PipeStage **pipe_stages;
    PipeOpReg *pipe_regs;
    Sram *nbin;
    Sram *sb;
    Sram *nbout;
    Dram *dram;
    std::deque<LoadStoreOp *> load_requests;
    std::deque<LoadStoreOp *> store_requests;

    datapath_mode mode;

    int64_t tot_op_issue;
    int64_t tot_op_complete;
};

#endif
