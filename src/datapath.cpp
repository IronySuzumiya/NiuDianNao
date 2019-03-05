#include "datapath.hpp"

Datapath::Datapath(DnnConfig *cfg) {

}

Datapath::~Datapath() {

}

void Datapath::tick() {

}

void Datapath::print_stats() {

}

void Datapath::print_pipeline() {

}

bool Datapath::is_all_needed_data_in_sram() {

}

bool Datapath::is_nbout_writeback_complete() {
    
}

void Datapath::push_pipe_op(PipeOp *op) {

}

bool Datapath::load_nbin(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {

}

bool Datapath::load_sb(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {

}

bool Datapath::store_nbout(mem_addr sram_addr, mem_addr dram_addr, mem_size size) {

}

bool Datapath::read_nbin(SramOp *sram_op) {

}

bool Datapath::read_sb(SramOp *sram_op) {

}

bool Datapath::read_nbout(SramOp *sram_op) {

}

bool Datapath::write_nbout(SramOp *sram_op) {

}
