#ifndef __NFU1__
#define __NFU1__

#include "pipe_stage.hpp"

class PipeStageNFU1 : public PipeStage {
public:
    PipeStageNFU1(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    ~PipeStageNFU1();

    void tick();
    void print();
};

#endif
