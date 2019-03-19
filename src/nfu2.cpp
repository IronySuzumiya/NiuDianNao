#include "nfu2.hpp"

using namespace std;

PipeStageNFU2::PipeStageNFU2(PipeOpReg *reg_in, PipeOpReg *reg_out,
                int queue_size, int n_stages,  unsigned num_adders,
                unsigned num_shifters, unsigned num_max, Sram *nbout)
                : PipeStage(reg_in, reg_out, queue_size, n_stages),
                num_adders(num_adders), num_shifters(num_shifters),
                num_max(num_max), nbout(nbout) {
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

    is_to_nbout = true;
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

void PipeStageNFU2::to_nbout() {
    is_to_nbout = true;
}

void PipeStageNFU2::to_nfu3() {
    is_to_nbout = false;
}

void PipeStageNFU2::tick() {
    PipeStage::tick();
    if(is_to_nbout) {
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

void PipeStageNFU2::print() {
    PipeStage::print();
    if(is_to_nbout) {
        if(reg_out->empty()) {
            cout << "No active NBout write." << endl;
        } else {
            cout << "Active NBout write: | ";
            for(PipeOpReg::iterator it = reg_out->begin(); it != reg_out->end(); ++it) {
                cout << (*it)->serial_num;
                if((*it)->is_pending) {
                    cout << "*";
                }
                cout << " | ";
            }
            cout << endl;
        }
    }
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

bool PipeStageNFU2::is_ready_to_fetch(PipeOp *op) {
    return op->is_ready_to_nfu2();
}

void PipeStageNFU2::preprocess_op(PipeOp *op) {
    read_nbout(&op->nbout_read_op);
}

void PipeStageNFU2::read_nbout(SramOp *op) {
    nbout->push_request(op);
}

void PipeStageNFU2::write_nbout(SramOp *op) {
    nbout->push_request(op);
}
