#include "nfu3.hpp"

using namespace std;

PipeStageNFU3::PipeStageNFU3(PipeOpReg *reg_in, PipeOpReg *reg_out,
                int queue_size, int n_stages,  unsigned num_multipliers, 
                unsigned num_adders)
                : PipeStage(reg_in, reg_out, queue_size, n_stages),
                num_multipliers(num_multipliers), num_adders(num_adders) {
    name = "NFU3";
    multipliers = new FunctionalUnit*[num_multipliers];
    for(unsigned i = 0; i < num_multipliers; ++i){
        multipliers[i] = new FunctionalUnit();
    }

    adders = new FunctionalUnit*[num_adders];
    for(unsigned i = 0; i < num_adders; ++i){
        adders[i] = new FunctionalUnit();
    }
}

PipeStageNFU3::~PipeStageNFU3() {
    for(unsigned i = 0; i < num_multipliers; ++i) {
        delete multipliers[i];
    }
    delete[] multipliers;

    for(unsigned i = 0; i < num_adders; ++i) {
        delete adders[i];
    }
    delete[] adders;
}

void PipeStageNFU3::do_op() {
    for(unsigned i = 0; i < num_multipliers; i++) {
        multipliers[i]->do_op();
    }
    for(unsigned i = 0; i < num_adders; i++) {
        adders[i]->do_op();
    }
}
