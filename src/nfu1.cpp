#include "nfu1.hpp"

using namespace std;

PipeStageNFU1::PipeStageNFU1(PipeOpReg *reg_in, PipeOpReg *reg_out,
                int queue_size, int n_stages, unsigned num_multipliers)
                : PipeStage(reg_in, reg_out, queue_size, n_stages),
                num_multipliers(num_multipliers) {
    name = "NFU1";
    multipliers = new FunctionalUnit*[num_multipliers];
    for(unsigned i = 0; i < num_multipliers; ++i){
        multipliers[i] = new FunctionalUnit();
    }
}

PipeStageNFU1::~PipeStageNFU1() {
    for(unsigned i = 0; i < num_multipliers; ++i){
        delete multipliers[i];
    }
    delete[] multipliers;
}

void PipeStageNFU1::do_op() {
    for(unsigned i = 0; i < num_multipliers; i++) {
        multipliers[i]->do_op();
    }
}
