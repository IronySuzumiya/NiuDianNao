#include "dram.hpp"

using namespace std;

Dram::Dram(const string& dram_config_file, 
            const string& system_config, 
            const string& dram_sim_dir,
            const string& prog_name,
            mem_size memory_size) {
    dram_sim = DRAMSim::getMemorySystemInstance(dram_config_file, 
                                            system_config, 
                                            dram_sim_dir, 
                                            prog_name, 
                                            memory_size);
    dram_sim->setCPUClockSpeed(0);
    read_callback = new DRAMSim::Callback<Dram, void, unsigned, uint64_t, uint64_t>(this, &Dram::read_complete_callback);
    write_callback = new DRAMSim::Callback<Dram, void, unsigned, uint64_t, uint64_t>(this, &Dram::write_complete_callback);
    dram_sim->RegisterCallbacks(read_callback, write_callback, NULL);
}

Dram::~Dram() {
    delete read_callback;
    delete write_callback;
    delete dram_sim;
}

void Dram::tick() {
    if(!new_requests.empty()) {
        do {
            if(dram_sim->willAcceptTransaction()) {
                #if FAST_DRAM_ACCESS
                DramOp *op;
                #else
                SubDramOp *op;
                #endif
                op = new_requests.front();
                new_requests.pop();
                dram_sim->addTransaction(
                #if FAST_DRAM_ACCESS
                    !op->is_read,
                #else
                    !op->owner->is_read,
                #endif
                    op->addr);
                active_requests.push_back(op);
            } else {
                break;
            }
        } while(!new_requests.empty());
    }
    dram_sim->update();
}

void Dram::push_request(DramOp *op) {
    #if FAST_DRAM_ACCESS
    new_requests.push(op);
    #else
    assert(!(op->size % 64));
    for(int i = 0; i < op->size; i += 64) {
        SubDramOp *sop = new SubDramOp({op, op->addr + i});
        op->sub_ops.push_back(sop);
        new_requests.push(sop);
    }
    original_requests.push_back(op);
    #endif
}

void Dram::read_complete_callback(unsigned id, uint64_t addr, uint64_t clock_cycle) {
    cout << "DRAM READ is complete: addr = "  <<  addr << endl;

    erase_finished_request(addr);
}

void Dram::write_complete_callback(unsigned id, uint64_t addr, uint64_t clock_cycle) {
    cout << "DRAM READ is complete: addr = "  <<  addr << endl;

    erase_finished_request(addr);
}

bool Dram::is_working() {
    return !active_requests.empty() || !new_requests.empty()
    #if !FAST_DRAM_ACCESS
    || !original_requests.empty()
    #endif
    ;
}

#if FAST_DRAM_ACCESS
void Dram::erase_finished_request(uint64_t addr) {
    DramReqDeque::iterator it;

    for(it = active_requests.begin(); it != active_requests.end(); ++it) {
        if(addr == (*it)->addr) {
            active_requests.erase(it);
            (*it)->is_complete = true;
            break;
        }
    }
}
#else
void Dram::erase_finished_request(uint64_t addr) {
    DramReqDeque::iterator it;

    for(it = active_requests.begin(); it != active_requests.end(); ++it) {
        if(addr == (*it)->addr) {
            DramOp *owner = (*it)->owner;

            active_requests.erase(it);
            
            DramReqDeque::iterator o_it;
            for(o_it = owner->sub_ops.begin(); o_it != owner->sub_ops.end(); ++o_it) {
                if(addr == (*o_it)->addr) {
                    delete *o_it;
                    owner->sub_ops.erase(o_it);
                    break;
                }
            }

            if(owner->sub_ops.empty()) {
                owner->is_complete = true;

                cout << "DRAM Bulk READ is complete: addr = " << owner->addr;
                cout << ", size = " << owner->size << endl;

                deque<DramOp *>::iterator o_o_it;
                for(o_o_it = original_requests.begin(); o_o_it != original_requests.end(); ++o_o_it) {
                    if(owner->addr == (*o_o_it)->addr) {
                        original_requests.erase(o_o_it);
                        break;
                    }
                }
            }

            break;
        }
    }
}
#endif
