#ifndef __NFU2__
#define __NFU2__

#include "pipestage.hh"

class NFU2 : public PipeStage {
    NFU2(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    virtual ~NFU2();

    void cycle();
    void print();
};

#endif