#ifndef __DRAM__
#define __DRAM__

#include "common.hh"
#include "../DRAMSim2/DRAMSim.h"

using namespace std;

class Dram {
public:
    Dram(const string& dram_config_file,
                    const string& system_config,
                    const string& dram_sim_dir,
                    const string& prog_name,
                    mem_size memory_size);
    ~Dram();
    void tick();
    bool can_accept_request() const;
    void push_request(mem_addr addr, bool is_write);
    void set_callbacks(DRAMSim::TransactionCompleteCB *read_callback, 
                       DRAMSim::TransactionCompleteCB *write_callback);
private:
    DRAMSim::MultiChannelMemorySystem *dram_sim;
};

#endif