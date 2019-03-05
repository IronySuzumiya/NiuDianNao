#ifndef __NFU2__
#define __NFU2__

#include "pipe_stage.hpp"

class PipeStageNFU2 : public PipeStage {
public:
    PipeStageNFU2(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    ~PipeStageNFU2();

    void tick();
    void print();
};

#endif