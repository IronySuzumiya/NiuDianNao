#ifndef __DATA_PATH__
#define __DATA_PATH__

#include "sram.hh"
#include "nfu1.hh"
#include "nfu2.hh"
#include "nfu3.hh"

typedef struct load_store_op {
    Sram *sram;
    DramOp dram_op;
    SramOp sram_op;
    bool is_load;
    bool has_been_sent_to_dram;
    bool is_complete;
} LoadStoreOp;

typedef std::queue<LoadStoreOp> LoadStoreOpReg;

class Datapath {
public:
    Datapath(DnnConfig *cfg);
    
    ~Datapath();

    void tick();

    void print_stats();
    void print_pipeline();

    bool is_ready();

    void push_pipe_op(PipeOp *op);
    bool load_nbin(mem_addr dram_addr, mem_addr sram_addr, mem_size size);
    bool load_sb(mem_addr dram_addr, mem_addr sram_addr, mem_size size);
    bool store_nbout(mem_addr sram_addr, mem_addr dram_addr, mem_size size);

private:
    DnnConfig *cfg;
    
    bool check_nb_out_complete();

    bool read_nbin(SramOp *sram_op);
    bool read_sb(SramOp *sram_op);
    bool read_nbout(SramOp *sram_op);
    bool write_nbout(SramOp *sram_op);
    
    PipeStage **pipe_stages;
    PipeOpReg *pipe_regs;
    Sram *nbin;
    Sram *sb;
    Sram *nbout;
    Dram *dram;
    LoadStoreOpReg requests;
    
    int reg_size;
};

#endif
