#ifndef __FUNCTIONAL_UNIT__
#define __FUNCTIONAL_UNIT__

#include "common.hpp"

class FunctionalUnit {
public:
    FunctionalUnit() : num_ops(0) {}
    virtual ~FunctionalUnit() {}
    
    virtual void do_op();
    long long get_stats();

protected:
    long long num_ops;
};

#endif