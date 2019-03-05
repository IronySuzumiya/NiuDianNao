#include "pipe_stage.hpp"

using namespace std;

PipeOp::PipeOp(mem_addr nbin_addr, mem_size nbin_size,
        mem_addr sb_addr, mem_size sb_size,
        mem_addr nbout_addr, mem_size nbout_size) {
    nbin_sram_op.addr = nbin_addr;
    nbin_sram_op.size = nbin_size;
    nbin_sram_op.is_read = true;
    nbin_sram_op.is_complete = false;

    sb_sram_op.addr = sb_addr;
    sb_sram_op.size = sb_size;
    sb_sram_op.is_read = true;
    sb_sram_op.is_complete = false;

    nbout_sram_op.addr = nbout_addr;
    nbout_sram_op.size = nbout_size;
    nbout_sram_op.is_read = true;
    nbout_sram_op.is_complete = false;

    serial_num = global_serial_num++;
}

bool PipeOp::data_is_ready() {
    nbin_sram_op.is_complete = true;
    sb_sram_op.is_complete = true;
}

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
