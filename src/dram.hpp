#ifndef __DRAM__
#define __DRAM__

#include "common.hpp"
#include "../DRAMSim2/DRAMSim.h"

#define MAKE_DRAM_OP(addr, size, is_read) {(addr), (size), (is_read), false}
#define MAKE_DRAM_READ(addr, size) MAKE_DRAM_OP((addr), (size), true)
#define MAKE_DRAM_WRITE(addr, size) MAKE_DRAM_OP((addr), (size), false)

struct DramOp {
    mem_addr addr;
    mem_size size;
    bool is_read;
    bool is_complete;
    // Since the DRAM Databus Bitwidth is set to 64 bytes,
    // we should separate a large DRAM request into several,
    // and send them to DRAM one by one.
    // But the whole process seems to be so slow,
    // I wonder if it's appropriate or not.
    std::deque<DramOp *> sub_ops;
};

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