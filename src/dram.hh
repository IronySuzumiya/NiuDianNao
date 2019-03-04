#ifndef __DRAM__
#define __DRAM__

#include "common.hh"
#include "../DRAMSim2/DRAMSim.h"

typedef struct dram_op {
    mem_addr addr;
    mem_size size;
    bool is_read;
    bool is_complete;
} DramOp;

typedef std::deque<DramOp *> DramOpReg;

class Dram {
public:
    Dram(const std::string& dram_config_file,
                    const std::string& system_config,
                    const std::string& dram_sim_dir,
                    const std::string& prog_name,
                    mem_size memory_size);
    ~Dram();
    void tick();
    bool can_accept_request() const;
    void push_request(DramOp *op);
    void Dram::read_complete_callback(unsigned id, uint64_t address, uint64_t clock_cycle);
    void Dram::write_complete_callback(unsigned id, uint64_t address, uint64_t clock_cycle);

private:
    DRAMSim::MultiChannelMemorySystem *dram_sim;
    DRAMSim::TransactionCompleteCB *read_callback;
    DRAMSim::TransactionCompleteCB *write_callback;
    DramOpReg requests;
};

#endif