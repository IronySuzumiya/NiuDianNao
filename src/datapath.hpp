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

class Datapath {
public:
    Datapath(DnnConfig *cfg);
    
    ~Datapath();

    void tick();

    void print_pipeline();

    bool is_ready();

    void push_pipe_op(PipeOp *op);

    void load_nbin(mem_addr dram_addr, mem_addr sram_addr, mem_size size);
    void load_sb(mem_addr dram_addr, mem_addr sram_addr, mem_size size);
    void store_nbout(mem_addr dram_addr, mem_addr sram_addr, mem_size size);

    void activate_nfu3();
    void deactivate_nfu3();

    void switch_nfu2_to_add_mode();
    void switch_nfu2_to_max_mode();

    bool is_working();
    bool is_dram_working();

private:
    DnnConfig *cfg;
    
    PipeStage **pipe_stages;
    PipeStageNFU1 *nfu1;
    PipeStageNFU2 *nfu2;
    PipeStageNFU3 *nfu3;
    PipeOpReg *pipe_regs;
    Sram *nbin;
    Sram *sb;
    Sram *nbout;
    Dram *dram;
    std::deque<LoadStoreOp *> load_requests;
    std::deque<LoadStoreOp *> store_requests;

    int64_t tot_op_issue;
    int64_t tot_op_complete;
};

#endif
