#ifndef __NIUDIANNAO__
#define __NIUDIANNAO__

#include "control_processor.hpp"
#include "datapath.hpp"

class NiuDianNao {
public:
    NiuDianNao(DnnConfig *cfg);
    ~NiuDianNao();
    void run();

private:
    void tick();

    ControlProcessor *cp;
    Datapath *dp;
    int64_t cycle;
};

#endif
