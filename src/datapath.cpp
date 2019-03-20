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

    dram = new Dram("ini/DDR2_micron_16M_8b_x8_sg3E.ini",
                    "system.ini", "./DRAMSim2/", "ndn", 16384);
    
    nfu1 = new PipeStageNFU1(&pipe_regs[0], &pipe_regs[1], max_buffer_size, cfg->num_nfu1_pipeline_stages,
                            cfg->num_nfu1_multipliers, nbin, sb);
    nfu2 = new PipeStageNFU2(&pipe_regs[1], &pipe_regs[2], max_buffer_size, cfg->num_nfu2_pipeline_stages,
                            cfg->num_nfu2_adders, cfg->num_nfu2_shifters, cfg->num_nfu2_max, nbout);
    nfu3 = new PipeStageNFU3(&pipe_regs[2], &pipe_regs[3], max_buffer_size, cfg->num_nfu3_pipeline_stages,
                            cfg->num_nfu3_multipliers, cfg->num_nfu3_adders, nbout);

    pipe_stages[0] = nfu1;
    pipe_stages[1] = nfu2;
    pipe_stages[2] = nfu3;
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
    delete [] pipe_stages;
    delete nfu1;
    delete nfu2;
    delete nfu3;
    delete [] pipe_regs;
    delete nbin;
    delete sb;
    delete nbout;
    delete dram;
}

void Datapath::tick() {
    dram->tick();

    std::deque<LoadStoreOp *>::iterator it;
    for(it = load_requests.begin(); it != load_requests.end(); ) {
        if(!(*it)->is_sent) {
            if((*it)->dram_op.is_complete) {
                (*it)->sram->push_request(&(*it)->sram_op);
                (*it)->is_sent = true;
            }
            ++it;
        } else if((*it)->sram_op.is_complete) {
            delete *it;
            it = load_requests.erase(it);
        } else {
            ++it;
        }
    }

    for(it = store_requests.begin(); it != store_requests.end(); ) {
        if(!(*it)->is_sent) {
            if((*it)->sram_op.is_complete) {
                dram->push_request(&(*it)->dram_op);
                (*it)->is_sent = true;
            }
            ++it;
        } else if((*it)->dram_op.is_complete) {
            delete *it;
            it = store_requests.erase(it);
        } else {
            ++it;
        }
    }

    nbin->tick();
    sb->tick();
    nbout->tick();

    for(int i = 2; i >= 0; --i) {
        pipe_stages[i]->tick();
    }
}

void Datapath::print_pipeline() {
    for(int i = 0; i < 3; ++i) {
        pipe_stages[i]->print();
    }
}

bool Datapath::is_ready() {
    return load_requests.empty();
}

void Datapath::push_pipe_op(PipeOp *op) {
    pipe_regs[0].push_back(op);
    ++tot_op_issue;
}

void Datapath::load_nbin(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    LoadStoreOp *op = new LoadStoreOp({nbin,
        MAKE_DRAM_READ(dram_addr, size),
        MAKE_SRAM_WRITE(sram_addr, size),
        false});
    load_requests.push_back(op);
    dram->push_request(&op->dram_op);
}

void Datapath::load_sb(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    LoadStoreOp *op = new LoadStoreOp({sb,
        MAKE_DRAM_READ(dram_addr, size),
        MAKE_SRAM_WRITE(sram_addr, size),
        false});
    load_requests.push_back(op);
    dram->push_request(&op->dram_op);
}

void Datapath::store_nbout(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    LoadStoreOp *op = new LoadStoreOp({nbout,
        MAKE_DRAM_WRITE(dram_addr, size),
        MAKE_SRAM_READ(sram_addr, size),
        false});
    store_requests.push_back(op);
    nbout->push_request(&op->sram_op);
}

void Datapath::activate_nfu3() {
    nfu3->activate();
    nfu2->to_nfu3();
}

void Datapath::deactivate_nfu3() {
    nfu3->deactivate();
    nfu2->to_nbout();
}

void Datapath::switch_nfu2_to_add_mode() {
    nfu2->to_add_mode();
}

void Datapath::switch_nfu2_to_max_mode() {
    nfu2->to_max_mode();
}

bool Datapath::is_working() {
    for(int i = 0; i < 4; ++i) {
        if(!pipe_regs[i].empty()) {
            return true;
        }
    }
    for(int i = 0; i < 3; ++i) {
        if(!pipe_stages[i]->is_pipe_line_empty()) {
            return true;
        }
    }
    return !load_requests.empty() || !store_requests.empty()
        || nbin->is_working() || sb->is_working() || nbout->is_working();
}

bool Datapath::is_dram_working() {
    return dram->is_working();
}
