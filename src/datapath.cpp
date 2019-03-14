#include "datapath.hpp"

using namespace std;

Datapath::Datapath(DnnConfig *cfg) {
    unsigned max_buffer_size = cfg->max_buffer_size;
    unsigned bit_width = cfg->bit_width;
    unsigned bytes = (bit_width / 8);

    pipe_stages = new PipeStage*[3];
    pipe_regs = new PipeOpReg[4];

    nbin = new Sram("NBin", cfg->nbin_line_length * bytes, cfg->nbin_num_lines, bit_width,
            cfg->nbin_num_ports, cfg->nbin_access_cycles);
    sb = new Sram("SB", cfg->sb_line_length * bytes, cfg->sb_num_lines, bit_width,
            cfg->sb_num_ports, cfg->sb_access_cycles);
    nbout = new Sram("NBout", cfg->nbout_line_length * bytes, cfg->nbout_num_lines, bit_width,
            cfg->nbout_num_ports, cfg->nbout_access_cycles);
    
    pipe_stages[0] = new PipeStageNFU1(&pipe_regs[0], &pipe_regs[1], max_buffer_size, cfg->num_nfu1_pipeline_stages, cfg->num_nfu1_multipliers);
    pipe_stages[1] = new PipeStageNFU2(&pipe_regs[1], &pipe_regs[2], max_buffer_size, cfg->num_nfu2_pipeline_stages, cfg->num_nfu2_adders, cfg->num_nfu2_shifters, cfg->num_nfu2_max);
    pipe_stages[2] = new PipeStageNFU3(&pipe_regs[2], &pipe_regs[3], max_buffer_size, cfg->num_nfu3_pipeline_stages, cfg->num_nfu3_multipliers, cfg->num_nfu3_adders);

    tot_op_issue = 0;
    tot_op_complete = 0;

    mode = nfu2_to_nbout;
}

Datapath::~Datapath() {
    for(int i = 0; i < 4; ++i) {
        if(!pipe_regs[i].empty()) {
            PipeOpReg::iterator it;
            for(it = pipe_regs[i].begin(); it != pipe_regs[i].end(); ++it) {
                delete *it;
            }
            pipe_regs[i].clear();
        }
    }
    for(int i = 0; i < 3; ++i) {
        delete pipe_stages[i];
    }
    delete [] pipe_stages;
    delete [] pipe_regs;
    delete [] nbin;
    delete [] sb;
    delete [] nbout;
}

void Datapath::tick() {
    for(PipeOpReg::iterator it = pipe_regs[0].begin(); it != pipe_regs[0].end(); ++it) {
        if((*it)->data_is_ready()) {
            assert((*it)->is_pending);
            (*it)->is_pending = false;
        } else if(!(*it)->is_pending) {
            read_nbin(&(*it)->nbin_sram_op);
            read_sb(&(*it)->sb_sram_op);
            (*it)->is_pending = true;
        }
    }

    for(int i = 0; i < 3; ++i) {
        pipe_stages[i]->tick();
    }

    int k = mode == nfu2_to_nbout ? 2 : 3;
    for(PipeOpReg::iterator it = pipe_regs[k].begin(); it != pipe_regs[k].end(); ++it) {
        if((*it)->write_is_complete()) {
            assert((*it)->is_pending);
            delete *it;
            pipe_regs[k].erase(it);
        } else if(!(*it)->is_pending) {
            write_nbout(&(*it)->nbout_sram_op);
            (*it)->is_pending = true;
        }
    }
    
    std::deque<LoadStoreOp *>::iterator it;
    for(it = requests.begin(); it != requests.end(); ++it) {
        if(!(*it)->is_sent) {
            if((*it)->is_load) {
                if((*it)->dram_op.is_complete) {
                    (*it)->sram->push_request(&(*it)->sram_op);
                    (*it)->is_sent = true;
                }
            } else {
                if((*it)->sram_op.is_complete) {
                    dram->push_request(&(*it)->dram_op);
                    (*it)->is_sent = true;
                }
            }
        } else if((*it)->is_load? (*it)->sram_op.is_complete : (*it)->dram_op.is_complete) {
            delete *it;
            requests.erase(it);
        }
    }
}

void Datapath::print_stats() {
    cout << "=============Stats=============" << endl;
    cout << "Total operations issued: " << tot_op_issue << endl;
    cout << "Total operations completed: " << tot_op_complete << endl;
    cout << "===============================" << endl;
}

void Datapath::print_pipeline() {
    int k = mode == nfu2_to_nbout ? 2 : 3;
    for(int i = 0; i < k; ++i) {
        cout << "NFU" << i + 1 << ": ";
        pipe_stages[i]->print();
    }
    cout << "Active Nbout Write: " << pipe_regs[k].front()->serial_num << endl;
}

bool Datapath::is_ready() {
    return requests.empty();
}

void Datapath::push_pipe_op(PipeOp *op) {
    pipe_regs[0].push_back(op);
}

void Datapath::load_nbin(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    LoadStoreOp *op = new LoadStoreOp({nbin,
        MAKE_DRAM_OP(dram_addr, size, true),
        MAKE_SRAM_OP(sram_addr, size, false),
        true, false});
    requests.push_back(op);
    dram->push_request(&op->dram_op);
}

void Datapath::load_sb(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    LoadStoreOp *op = new LoadStoreOp({sb,
        MAKE_DRAM_OP(dram_addr, size, true),
        MAKE_SRAM_OP(sram_addr, size, false),
        true, false});
    requests.push_back(op);
    dram->push_request(&op->dram_op);
}

void Datapath::store_nbout(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    LoadStoreOp *op = new LoadStoreOp({nbout,
        MAKE_DRAM_OP(dram_addr, size, false),
        MAKE_SRAM_OP(sram_addr, size, true),
        false, false});
    requests.push_back(op);
    nbout->push_request(&op->sram_op);
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
