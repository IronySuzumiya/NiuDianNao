#ifndef __NFU1__
#define __NFU1__

#include "pipestage.hh"

class NFU1 : public PipeStage {
    NFU1(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages);
    virtual ~NFU1();

    void cycle();
    void print();
};

#endif
