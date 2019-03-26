#include "pipe_op.hpp"

PipeOp::PipeOp(mem_addr nbin_addr, mem_size nbin_size,
        mem_addr sb_addr, mem_size sb_size,
        mem_addr nbout_addr, mem_size nbout_size,
        bool is_partial_sum)
        : nbin_read_op(MAKE_SRAM_READ(nbin_addr, nbin_size)),
        sb_read_op(MAKE_SRAM_READ(sb_addr, sb_size)),
        nbout_read_op(MAKE_SRAM_READ(nbout_addr, nbout_size)),
        serial_num(global_serial_num++), is_pending(false),
        is_partial_sum(is_partial_sum) {
    if(is_partial_sum) {
        nbout_write_op = MAKE_SRAM_WRITE_PARTIAL(nbout_addr, nbout_size);
    } else {
        nbout_write_op = MAKE_SRAM_WRITE_FINAL(nbout_addr, nbout_size);
    }
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
