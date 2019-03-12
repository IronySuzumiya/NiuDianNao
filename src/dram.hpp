#ifndef __DRAM__
#define __DRAM__

#include "common.hpp"
#include "../DRAMSim2/DRAMSim.h"

#define MAKE_DRAM_OP(addr, size, is_read) {(addr), (size), (is_read), false}

typedef struct dram_op {
    mem_addr addr;
    mem_size size;
    bool is_read;
    bool is_complete;
} DramOp;

class Dram {
public:
    Dram(const std::string& dram_config_file,
                    const std::string& system_config,
                    const std::string& dram_sim_dir,
                    const std::string& prog_name,
                    mem_size memory_size);
    ~Dram();
    void tick();
    void push_request(DramOp *op);
    void read_complete_callback(unsigned id, uint64_t address, uint64_t clock_cycle);
    void write_complete_callback(unsigned id, uint64_t address, uint64_t clock_cycle);

private:
    DRAMSim::MultiChannelMemorySystem *dram_sim;
    DRAMSim::TransactionCompleteCB *read_callback;
    DRAMSim::TransactionCompleteCB *write_callback;
    std::queue<DramOp *> new_requests;
    std::deque<DramOp *> active_requests;
};

#endif