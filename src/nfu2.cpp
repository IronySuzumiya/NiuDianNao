#include "nfu2.hpp"

using namespace std;

PipeStageNFU2::PipeStageNFU2(PipeOpReg *reg_in, PipeOpReg *reg_out,
                int queue_size, int n_stages,  unsigned num_adders,
                unsigned num_shifters, unsigned num_max)
                : PipeStage(reg_in, reg_out, queue_size, n_stages),
                num_adders(num_adders), num_shifters(num_shifters),
                num_max(num_max) {
    name = "NFU2";
    adders = new FunctionalUnit*[num_adders];
    for(unsigned i = 0; i < num_adders; ++i){
        adders[i] = new FunctionalUnit();
    }

    shifters = new FunctionalUnit*[num_shifters];
    for(unsigned i = 0; i < num_shifters; ++i){
        shifters[i] = new FunctionalUnit();
    }
    
    max = new FunctionalUnit*[num_max];
    for(unsigned i = 0; i < num_max; ++i){
        max[i] = new FunctionalUnit();
    }
}

PipeStageNFU2::~PipeStageNFU2() {
    for(unsigned i = 0; i < num_adders; ++i) {
        delete adders[i];
    }
    delete[] adders;

    for(unsigned i = 0; i < num_shifters; ++i) {
        delete shifters[i];
    }
    delete[] shifters;

    for(unsigned i = 0; i < num_max; ++i) {
        delete max[i];
    }
    delete[] max;
}

void PipeStageNFU2::do_op() {
    for(unsigned i = 0; i < num_adders; i++) {
        adders[i]->do_op();
    }
    for(unsigned i = 0; i < num_shifters; i++) {
        shifters[i]->do_op();
    }
    for(unsigned i = 0; i < num_max; i++) {
        max[i]->do_op();
    }
}
