#ifndef __NFU3__
#define __NFU3__

#include "pipe_stage.hpp"

class PipeStageNFU3 : public PipeStage {
public:
    PipeStageNFU3(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    ~PipeStageNFU3();

    void tick();
    void print();
};

#endif