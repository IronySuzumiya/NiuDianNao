#ifndef __DATA_PATH__
#define __DATA_PATH__

#include "sram.hh"
#include "nfu1.hh"
#include "nfu2.hh"
#include "nfu3.hh"

class Datapath {
public:
    Datapath(DnnConfig *cfg);
    
    ~Datapath();

    void tick();

    void print_stats();
    void print_pipeline();
    
private:
    DnnConfig *m_config;
    
    bool check_nb_out_complete();
    
    PipeStage **pipe_stages;
    pipe_reg *pipe_regs;
    Sram *nbin;
    Sram *sb;
    Sram *nbout;
    
    int reg_size;
};

#endif
