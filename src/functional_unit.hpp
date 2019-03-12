#ifndef __FUNCTIONAL_UNIT__
#define __FUNCTIONAL_UNIT__

#include "common.h"

class functional_unit {
public:
    functional_unit() : m_num_ops(0) {}
    ~functional_unit() {}
    
    void do_op();
    unsigned get_stats();

private:
    // Performance counter for the number of operations performed.
    unsigned m_num_ops;

};

#endif