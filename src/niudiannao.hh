#ifndef __NIUDIANNAO__
#define __NIUDIANNAO__

#include "dram.hh"
#include "sram.hh"
#include "cp.hh"
#include "nfu1.hh"
#include "nfu2.hh"
#include "nfu3.hh"

class NiuDianNao {
public:
    NiuDianNao();
    virtual ~NiuDianNao();
    void cycle();
private:
    Dram *dram;
    Sram *NBin;
    Sram *SB;
    Sram *NBout;
    CP* cp;
    NFU1 *nfu1;
    NFU2 *nfu2;
    NFU3 *nfu3;
};

#endif
