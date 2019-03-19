#include "dram.hpp"

Dram::Dram(const std::string& dram_config_file, 
            const std::string& system_config, 
            const std::string& dram_sim_dir,
            const std::string& prog_name,
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
                DramOp *op = new_requests.front();
                new_requests.pop();
                dram_sim->addTransaction(!op->is_read, op->addr);
                active_requests.push_back(op);
            }
        } while(!new_requests.empty());
    }
    dram_sim->update();
}

void Dram::push_request(DramOp *op) {
    new_requests.push(op);
}

void Dram::read_complete_callback(unsigned id, uint64_t addr, uint64_t clock_cycle) {
    std::cout << "DRAM Read callback for address "  <<  addr << " (cycle: " << clock_cycle << ")" << std::endl;

    std::deque<DramOp *>::iterator it;
    for(it = active_requests.begin(); it != active_requests.end(); ++it) {
        if(addr == (*it)->addr) {
            (*it)->is_complete = true;
            active_requests.erase(it);
            break;
        }
    }
}

void Dram::write_complete_callback(unsigned id, uint64_t addr, uint64_t clock_cycle) {
    std::cout << "DRAM Write callback for address "  <<  addr << " (cycle: " << clock_cycle << ")" << std::endl;

    std::deque<DramOp *>::iterator it;
    for(it = active_requests.begin(); it != active_requests.end(); ++it) {
        if(addr == (*it)->addr) {
            (*it)->is_complete = true;
            active_requests.erase(it);
            break;
        }
    }
}