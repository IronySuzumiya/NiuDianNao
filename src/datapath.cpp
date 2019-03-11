#include "datapath.hpp"
using namespace std;
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
    LoadStoreOp LSOp;
    if(LSOp.is_load == true) {
        return true;
    } else {
        return false;
    }
    
    
}

bool Datapath::is_nbout_writeback_complete() {
  

}

void Datapath::push_pipe_op(PipeOp *op) {

}

bool Datapath::load_nbin(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {

    
    if(dram->can_accept_request()) {
        DramOp *dram_op;
        dram_op->addr = dram_addr;
        dram_op->size = size;
        dram_op->is_read = true;
        dram_op->is_complete = false;
        dram->push_request(dram_op);
    }
           
}

bool Datapath::load_sb(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    
    if(dram->can_accept_request()) {
        DramOp *dram_op;
        dram_op->addr = dram_addr;
        dram_op->size = size;
        dram_op->is_read = true;
        dram_op->is_complete = false;
        dram->push_request(dram_op);
    }
         
}

bool Datapath::store_nbout(mem_addr sram_addr, mem_addr dram_addr, mem_size size) {
    if (dram->can_accept_request()) {
        DramOp *dram_op;
        dram_op->addr = dram_addr;
        dram_op->size = size;
        dram_op->is_read = false;
        dram_op->is_complete = false;
        dram->push_request(dram_op);
    }
    
}

bool Datapath::read_nbin(SramOp *sram_op) {
  
}

bool Datapath::read_sb(SramOp *sram_op) {

}

bool Datapath::read_nbout(SramOp *sram_op) {

}

bool Datapath::write_nbout(SramOp *sram_op) {

}
