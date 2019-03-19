#include "nfu3.hpp"

using namespace std;

PipeStageNFU3::PipeStageNFU3(PipeOpReg *reg_in, PipeOpReg *reg_out,
                int queue_size, int n_stages,  unsigned num_multipliers, 
                unsigned num_adders, Sram *nbout)
                : PipeStage(reg_in, reg_out, queue_size, n_stages),
                num_multipliers(num_multipliers), num_adders(num_adders),
                nbout(nbout) {
    name = "NFU3";
    multipliers = new FunctionalUnit*[num_multipliers];
    for(unsigned i = 0; i < num_multipliers; ++i){
        multipliers[i] = new FunctionalUnit();
    }

    adders = new FunctionalUnit*[num_adders];
    for(unsigned i = 0; i < num_adders; ++i){
        adders[i] = new FunctionalUnit();
    }

    is_activated = false;
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

void PipeStageNFU3::activate() {
    is_activated = true;
}

void PipeStageNFU3::deactivate() {
    is_activated = false;
}

void PipeStageNFU3::tick() {
    if(is_activated) {
        PipeStage::tick();
        for(PipeOpReg::iterator it = reg_out->begin(); it != reg_out->end(); ) {
            if((*it)->is_complete()) {
                assert((*it)->is_pending);
                delete *it;
                it = reg_out->erase(it);
            } else if(!(*it)->is_pending) {
                write_nbout(&(*it)->nbout_write_op);
                (*it)->is_pending = true;
                ++it;
            } else {
                ++it;
            }
        }
    }
}

void PipeStageNFU3::print() {
    if(is_activated) {
        PipeStage::print();
        print_reg_out_as_nbout_write();
    }
}

void PipeStageNFU3::do_op() {
    for(unsigned i = 0; i < num_multipliers; i++) {
        multipliers[i]->do_op();
    }
    for(unsigned i = 0; i < num_adders; i++) {
        adders[i]->do_op();
    }
}

bool PipeStageNFU3::is_ready_to_fetch(PipeOp *op) {
    return is_activated;
}

void PipeStageNFU3::preprocess_op(PipeOp *op) {
    write_nbout(&op->nbout_write_op);
}

void PipeStageNFU3::write_nbout(SramOp *op) {
    nbout->push_request(op);
}
