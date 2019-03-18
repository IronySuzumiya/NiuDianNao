#include "pipe_stage.hpp"

using namespace std;

PipeOp::PipeOp(mem_addr nbin_addr, mem_size nbin_size,
        mem_addr sb_addr, mem_size sb_size,
        mem_addr nbout_addr, mem_size nbout_size) {
    nbin_read_op.addr = nbin_addr;
    nbin_read_op.size = nbin_size;
    nbin_read_op.is_read = true;
    nbin_read_op.is_complete = false;

    sb_read_op.addr = sb_addr;
    sb_read_op.size = sb_size;
    sb_read_op.is_read = true;
    sb_read_op.is_complete = false;

    nbout_read_op.addr = nbout_addr;
    nbout_read_op.size = nbout_size;
    nbout_read_op.is_read = true;
    nbout_read_op.is_complete = false;

    nbout_write_op.addr = nbout_addr;
    nbout_write_op.size = nbout_size;
    nbout_write_op.is_read = false;
    nbout_write_op.is_complete = false;

    serial_num = global_serial_num++;

    is_pending = false;
}

bool PipeOp::is_ready_to_nfu1() {
    return nbin_read_op.is_complete && sb_read_op.is_complete;
}

bool PipeOp::is_ready_to_nfu2() {
    return nbout_read_op.is_complete;
}

bool PipeOp::is_complete() {
    return nbout_write_op.is_complete;
}

PipeStage::PipeStage(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages)
    : reg_in(reg_in), reg_out(reg_out), queue_size(queue_size), n_stages(n_stages) {
    pipeline = new PipeOp*[n_stages];
    for(int i = 0; i < n_stages; ++i) {
        pipeline[i] = NULL;
    }
    needs_data = false;
}

PipeStage::~PipeStage() {
    if(pipeline)
        delete [] pipeline;
}

bool PipeStage::is_pipe_op_reg_empty(PipeOpReg *reg) {
    return reg->size() == 0;
}

bool PipeStage::is_pipe_op_reg_full(PipeOpReg *reg) {
    return reg->size() >= queue_size;
}

void PipeStage::tick() {
    if (!is_pipe_op_reg_full(reg_out)) {
        if(pipeline[n_stages - 1]) {
            reg_out->push_back(pipeline[n_stages - 1]);
            do_op();
        }
        for(int i = n_stages - 1; i > 0; --i) {
            pipeline[i] = pipeline[i - 1];
        }
        pipeline[0] = NULL;
        PipeOpReg::iterator it;
        for(it = reg_in->begin(); it != reg_in->end(); ) {
            if(needs_data) {
                if(!(*it)->is_pending) {
                    (*it)->is_pending = true;
                    read_data((*it));
                    ++it;
                } else if(is_ready_to_fetch(*it)) {
                    assert(!pipeline[0]);
                    (*it)->is_pending = false;
                    pipeline[0] = (*it);
                    it = reg_in->erase(it);
                }
            } else {
                assert(!pipeline[0]);
                pipeline[0] = (*it);
                it = reg_in->erase(it);
            }
        }
    } else {
        cout << "Pipeline is stuck at " << name << "." << endl;
    }
}

void PipeStage::print() {
    if(is_pipe_op_reg_empty(reg_in)) {
        cout << "Reg_in is empty." << endl;
    } else {
        cout << "Reg_in: | ";
        for(PipeOpReg::reverse_iterator it = reg_in->rbegin(); it != reg_in->rend(); ++it) {
            cout << (*it)->serial_num << " | ";
        }
        cout << endl;
    }

    cout << "Pipeline: | ";
    for(int i = 0; i < n_stages; ++i) {
        PipeOp *op = pipeline[i];
        if(op) {
            cout << pipeline[i]->serial_num;
        } else {
            cout << " ";
        }
        cout << " | ";
    }
    cout << endl;
}
