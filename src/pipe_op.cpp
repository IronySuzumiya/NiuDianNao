#include "pipe_op.hpp"

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
