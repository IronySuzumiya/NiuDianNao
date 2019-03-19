#include "nfu1.hpp"

using namespace std;

PipeStageNFU1::PipeStageNFU1(PipeOpReg *reg_in, PipeOpReg *reg_out,
                int queue_size, int n_stages, unsigned num_multipliers,
                Sram *nbin, Sram *sb)
                : PipeStage(reg_in, reg_out, queue_size, n_stages),
                num_multipliers(num_multipliers), nbin(nbin), sb(sb) {
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

bool PipeStageNFU1::is_ready_to_fetch(PipeOp *op) {
    return op->is_ready_to_nfu1();
}

void PipeStageNFU1::preprocess_op(PipeOp *op) {
    read_nbin(&op->nbin_read_op);
    read_sb(&op->sb_read_op);
}

void PipeStageNFU1::read_nbin(SramOp *op) {
    nbin->push_request(op);
}

void PipeStageNFU1::read_sb(SramOp *op) {
    sb->push_request(op);
}
