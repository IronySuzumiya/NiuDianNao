#include "pipestage.hh"

using namespace std;

PipeStage::PipeStage(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages)
    : reg_in(reg_in), reg_out(reg_out), queue_size(queue_size), n_stages(n_stages) {
    pipeline = new PipeOp*[n_stages];
}

PipeStage::~PipeStage() {
    if(pipeline)
        delete [] pipeline;
}

bool PipeStage::push_op(PipeOp *op) {
    if(!is_pipe_op_reg_full(reg_in)) {
        reg_in->push(op);
        return true;
    } else {
        return false;
    }
}

bool PipeStage::is_pipe_op_reg_empty(PipeOpReg *reg) {
    return reg->size() == 0;
}

bool PipeStage::is_pipe_op_reg_full(PipeOpReg *reg) {
    return reg->size() >= queue_size;
}

void PipeStage::print() {
    if(is_pipe_op_reg_empty(reg_in)) {
        cout << "Reg_in is empty." << endl;
    } else {
        cout << "Next OP: " << reg_in->front()->get_serial_num() << endl;
    }

    cout << "Pipeline: | ";
    for(int i = 0; i < n_stages; ++i) {
        cout << pipeline[i]->get_serial_num() << " | ";
    }
    cout << endl;
}
