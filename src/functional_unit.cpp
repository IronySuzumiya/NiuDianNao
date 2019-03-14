#include "functional_unit.hpp"

void FunctionalUnit::do_op() {
    num_ops++;
}

long long FunctionalUnit::get_stats() {
    return num_ops;
}
