#include "datapath.hpp"

using namespace std;

Datapath::Datapath(DnnConfig *cfg) {
    
}

Datapath::~Datapath() {

}

void Datapath::tick() {
    // memory
    std::deque<LoadStoreOp>::iterator it;
    for(it = requests.begin(); it != requests.end(); ++it) {
        if(!it->is_sent) {
            if(it->is_load) {
                if(it->dram_op.is_complete) {
                    it->sram->push_request(&it->sram_op);
                    it->is_sent = true;
                }
            } else {
                if(it->sram_op.is_complete) {
                    dram->push_request(&it->dram_op);
                    it->is_sent = true;
                }
            }
        } else if(it->is_load? it->sram_op.is_complete : it->dram_op.is_complete) {
            requests.erase(it);
        }
    }
    // pipeline
    // TODO
}

void Datapath::print_stats() {
   
}

void Datapath::print_pipeline() {

}

bool Datapath::is_ready() {
    return requests.empty();
}

void Datapath::push_pipe_op(PipeOp *op) {
    pipe_regs[0].push(op);
}

void Datapath::load_nbin(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    requests.push_back({
        nbin, MAKE_DRAM_OP(dram_addr, size, true), MAKE_SRAM_OP(sram_addr, size, false),
        true, false
    });
    dram->push_request(&requests.back().dram_op);
}

void Datapath::load_sb(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    requests.push_back({
        sb, MAKE_DRAM_OP(dram_addr, size, true), MAKE_SRAM_OP(sram_addr, size, false),
        true, false
    });
    dram->push_request(&requests.back().dram_op);
}

void Datapath::store_nbout(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    requests.push_back({
        nbout, MAKE_DRAM_OP(dram_addr, size, false), MAKE_SRAM_OP(sram_addr, size, true),
        false, false
    });
    nbout->push_request(&requests.back().sram_op);
}

void Datapath::read_nbin(SramOp *op) {
    nbin->push_request(op);
}

void Datapath::read_sb(SramOp *op) {
    sb->push_request(op);
}

void Datapath::read_nbout(SramOp *op) {
    nbout->push_request(op);
}

void Datapath::write_nbout(SramOp *op) {
    nbout->push_request(op);
}
