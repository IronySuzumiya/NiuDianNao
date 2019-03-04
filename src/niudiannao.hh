#ifndef __NIUDIANNAO__
#define __NIUDIANNAO__

#include "dram.hh"
#include "sram.hh"
#include "control_processor.hh"
#include "datapath.hh"

class NiuDianNao {
public:
    NiuDianNao();
    virtual ~NiuDianNao();
    void tick();

private:
    Dram *dram;
    ControlProcessor *cp;
    Datapath *dp;
};

#endif
