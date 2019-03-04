#ifndef __NFU3__
#define __NFU3__

#include "pipestage.hh"

class NFU3 : public PipeStage {
    NFU3(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    virtual ~NFU3();

    void cycle();
    void print();
};

#endif