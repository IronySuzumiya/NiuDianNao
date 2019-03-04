#include "dram.hh"

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
}

Dram::~Dram() {
   delete dram_sim; 
}

void Dram::tick() {
    dram_sim->update();
}

bool Dram::can_accept_request() const {
    return dram_sim->willAcceptTransaction();
}

void Dram::push_request(mem_addr addr, bool is_write) {
    dram_sim->addTransaction(is_write, addr);
}

void Dram::set_callbacks(DRAMSim::TransactionCompleteCB *read_callback,
                        DRAMSim::TransactionCompleteCB *write_callback) {
    dram_sim->RegisterCallbacks(read_callback, write_callback, NULL);
}