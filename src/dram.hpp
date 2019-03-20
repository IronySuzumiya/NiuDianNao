#ifndef __DRAM__
#define __DRAM__

#include "common.hpp"
#include "../DRAMSim2/DRAMSim.h"

#define MAKE_DRAM_OP(addr, size, is_read) {(addr), (size), (is_read), false}
#define MAKE_DRAM_READ(addr, size) MAKE_DRAM_OP((addr), (size), true)
#define MAKE_DRAM_WRITE(addr, size) MAKE_DRAM_OP((addr), (size), false)

#if !FAST_DRAM_ACCESS
struct SubDramOp;
typedef std::queue<SubDramOp *> DramReqQueue;
typedef std::deque<SubDramOp *> DramReqDeque;
#endif

struct DramOp {
    mem_addr addr;
    mem_size size;
    bool is_read;
    bool is_complete;
    #if !FAST_DRAM_ACCESS
    DramReqDeque sub_ops;
    #endif
};

#if !FAST_DRAM_ACCESS
struct SubDramOp {
    DramOp *owner;
    mem_addr addr;
};
#endif

#if FAST_DRAM_ACCESS
typedef std::queue<DramOp *> DramReqQueue;
typedef std::deque<DramOp *> DramReqDeque;
#else

#endif

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

    bool is_working();

private:
    void erase_finished_request(uint64_t addr, bool is_read);

    DRAMSim::MultiChannelMemorySystem *dram_sim;
    DRAMSim::TransactionCompleteCB *read_callback;
    DRAMSim::TransactionCompleteCB *write_callback;
    #if !FAST_DRAM_ACCESS
    std::deque<DramOp *> original_requests;
    #endif
    DramReqQueue new_requests;
    DramReqDeque active_requests;
};

#endif