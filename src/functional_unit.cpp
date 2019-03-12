#include "functional_unit.h"

void functional_unit::do_op(){
    m_num_ops++;
}

unsigned functional_unit::get_stats(){
    return m_num_ops;
}