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
    
    pipe_stages[0] = new PipeStageNFU1(this, &pipe_regs[0], &pipe_regs[1], max_buffer_size, cfg->num_nfu1_pipeline_stages, cfg->num_nfu1_multipliers);
    pipe_stages[1] = new PipeStageNFU2(this, &pipe_regs[1], &pipe_regs[2], max_buffer_size, cfg->num_nfu2_pipeline_stages, cfg->num_nfu2_adders, cfg->num_nfu2_shifters, cfg->num_nfu2_max);
    pipe_stages[2] = new PipeStageNFU3(this, &pipe_regs[2], &pipe_regs[3], max_buffer_size, cfg->num_nfu3_pipeline_stages, cfg->num_nfu3_multipliers, cfg->num_nfu3_adders);

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
    delete nbin;
    delete sb;
    delete nbout;
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

    int k = mode == nfu2_to_nbout ? 2 : 3;

    for(int i = k - 1; i >= 0; --i) {
        pipe_stages[i]->tick();
    }
    
    for(PipeOpReg::iterator it = pipe_regs[k].begin(); it != pipe_regs[k].end(); ) {
        if((*it)->is_complete()) {
            assert((*it)->is_pending);
            delete *it;
            it = pipe_regs[k].erase(it);
            ++tot_op_complete;
        } else if(!(*it)->is_pending) {
            write_nbout(&(*it)->nbout_write_op);
            (*it)->is_pending = true;
            ++it;
        } else {
            ++it;
        }
    }

    nbout->tick();
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
    if(pipe_regs[k].empty()) {
        cout << "No active NBout write." << endl;
    } else {
        cout << "Active NBout write: | ";
        for(PipeOpReg::iterator it = pipe_regs[k].begin(); it != pipe_regs[k].end(); ++it) {
            cout << (*it)->serial_num << " | ";
        }
        cout << endl;
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
        MAKE_DRAM_OP(dram_addr, size, true),
        MAKE_SRAM_OP(sram_addr, size, false),
        false});
    load_requests.push_back(op);
    dram->push_request(&op->dram_op);
}

void Datapath::load_sb(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    LoadStoreOp *op = new LoadStoreOp({sb,
        MAKE_DRAM_OP(dram_addr, size, true),
        MAKE_SRAM_OP(sram_addr, size, false),
        false});
    load_requests.push_back(op);
    dram->push_request(&op->dram_op);
}

void Datapath::store_nbout(mem_addr dram_addr, mem_addr sram_addr, mem_size size) {
    LoadStoreOp *op = new LoadStoreOp({nbout,
        MAKE_DRAM_OP(dram_addr, size, false),
        MAKE_SRAM_OP(sram_addr, size, true),
        false});
    store_requests.push_back(op);
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
