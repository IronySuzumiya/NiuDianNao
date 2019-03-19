#include "pipe_op.hpp"

PipeOp::PipeOp(mem_addr nbin_addr, mem_size nbin_size,
        mem_addr sb_addr, mem_size sb_size,
        mem_addr nbout_addr, mem_size nbout_size)
        : nbin_read_op(MAKE_SRAM_READ(nbin_addr, nbin_size)),
        sb_read_op(MAKE_SRAM_READ(sb_addr, sb_size)),
        nbout_read_op(MAKE_SRAM_READ(nbout_addr, nbout_size)),
        nbout_write_op(MAKE_SRAM_WRITE(nbout_addr, nbout_size)),
        serial_num(global_serial_num++), is_pending(false) {
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
