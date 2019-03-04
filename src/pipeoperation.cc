#include "pipeoperation.hh"

PipeOp::PipeOp(mem_addr nbin_addr, mem_size nbin_size,
                mem_addr sb_addr, mem_size sb_size,
                mem_addr nbout_addr, mem_size nbout_size,
                int serial_num) {
    
    nbin_op.addr = nbin_addr;
    nbin_op.size = nbin_size;
    nbin_op.is_read = true;
    nbin_op.is_complete = false;

    sb_op.addr = sb_addr;
    sb_op.addr = sb_size;
    sb_op.is_read = true;
    sb_op.is_complete = false;

    nbout_op.addr = nbout_addr;
    nbout_op.addr = nbout_size;
    nbout_op.is_read = false;
    nbout_op.is_complete = false;

    this->serial_num = serial_num;

    this->is_read = false;
}

bool PipeOp::is_ready() {
    return is_read()? is_read_complete() : is_write_complete();
}

bool PipeOp::is_read() {
    return _is_read;
}

bool PipeOp::is_read_complete() {
    if(nbin_op.is_complete && sb_op.is_complete)
        return true;
    else
        return false;
}

bool PipeOp::is_write_complete() {
    if(nbout_op.is_complete)
        return true;
    else
        return false;
}

void PipeOp::set_read() {
    _is_read = true;
}

void PipeOp::set_write() {
    _is_read = false;
}

bool PipeOp::is_in_pipe_reg() {
    return _is_in_pipe_reg;
}

void PipeOp::set_in_pipe_reg() {
    _is_in_pipe_reg = true;
}

int PipeOp::get_serial_num() {
    return serial_num;
}
