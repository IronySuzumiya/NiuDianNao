#include "pipe_stage.hpp"

using namespace std;

PipeStage::PipeStage(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, int n_stages)
    : reg_in(reg_in), reg_out(reg_out), queue_size(queue_size), n_stages(n_stages) {
    pipeline = new PipeOp*[n_stages];
    for(int i = 0; i < n_stages; ++i) {
        pipeline[i] = NULL;
    }
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

void PipeStage::print_reg_out_as_nbout_write() {
    if(reg_out->empty()) {
        cout << "No active NBout write." << endl;
    } else {
        cout << "Active NBout write: | ";
        for(PipeOpReg::reverse_iterator it = reg_out->rbegin(); it != reg_out->rend(); ++it) {
            cout << (*it)->serial_num;
            if((*it)->is_pending) {
                cout << "*";
            }
            cout << " | ";
        }
        cout << endl;
    }
}

void PipeStage::tick() {
    if (!is_pipe_op_reg_full(reg_out)) {
        if(pipeline[n_stages - 1]) {
            do_op();
            reg_out->push_back(pipeline[n_stages - 1]);
        }
        for(int i = n_stages - 1; i > 0; --i) {
            pipeline[i] = pipeline[i - 1];
        }
        pipeline[0] = NULL;
        PipeOpReg::iterator it;
        for(it = reg_in->begin(); it != reg_in->end(); ) {
            if(is_ready_to_fetch(*it)) {
                assert(!pipeline[0]);
                (*it)->is_pending = false;
                pipeline[0] = (*it);
                it = reg_in->erase(it);
            } else if((*it)->is_complete()) {
                assert((*it)->is_pending);
                delete *it;
                it = reg_in->erase(it);
            } else if(!(*it)->is_pending) {
                (*it)->is_pending = true;
                preprocess_op((*it));
                ++it;
            } else {
                ++it;
            }
        }
    } else {
        cerr << "Pipeline is stuck at " << name << "." << endl;
        abort();
    }
}

void PipeStage::print() {
    cout << name << ": ";
    if(is_pipe_op_reg_empty(reg_in)) {
        cout << "Reg_in is empty." << endl;
    } else {
        cout << "Reg_in: | ";
        for(PipeOpReg::reverse_iterator it = reg_in->rbegin(); it != reg_in->rend(); ++it) {
            cout << (*it)->serial_num;
            if((*it)->is_pending) {
                cout << "*";
            }
            cout << " | ";
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

bool PipeStage::is_pipe_line_empty() {
    for(int i = 0; i < n_stages; ++i) {
        if(pipeline[i]) {
            return false;
        }
    }
    return true;
}
