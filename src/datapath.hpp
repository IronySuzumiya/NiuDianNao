#ifndef __DATA_PATH__
#define __DATA_PATH__

#include "dram.hpp"
#include "sram.hpp"
#include "pipe_stage.hpp"
#include "config.hpp"

typedef struct load_store_op {
    Sram *sram;
    DramOp dram_op;
    SramOp sram_op;
    bool is_load;
    bool is_sent;
} LoadStoreOp;

class Datapath {
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

private:
    DnnConfig *cfg;

    void read_nbin(SramOp *op);
    void read_sb(SramOp *op);
    void read_nbout(SramOp *op);
    void write_nbout(SramOp *op);
    
    PipeStage **pipe_stages;
    PipeOpReg *pipe_regs;
    Sram *nbin;
    Sram *sb;
    Sram *nbout;
    Dram *dram;
    std::deque<LoadStoreOp> requests;
    
    int reg_size;
};

#endif
