#ifndef __SRAM__
#define __SRAM__

#include "common.hpp"

#define MAKE_SRAM_OP(addr, size, is_read) {(addr), (size), (is_read), false}
#define MAKE_SRAM_READ(addr, size) MAKE_SRAM_OP((addr), (size), true)
#define MAKE_SRAM_WRITE(addr, size) MAKE_SRAM_OP((addr), (size), false)

struct SramOp {
    mem_addr addr;
    mem_size size;
    bool is_read;
    bool is_complete;
};

struct SramLine {
    bool valid;             // for consistency
    bool is_partial_sum;    // partial sum can be overwritten
};

struct SramPort {
    bool is_busy;
    int cur_access_cycle;
    SramOp *op;
};

typedef std::queue<SramOp *> SramOpReg;

class Sram {
public:
    Sram(const std::string& name, int line_size, int num_lines, int bit_width,
            int num_read_write_ports, int num_cycle_per_access);
    ~Sram();
    
    void tick();

    void push_request(SramOp *op);

    bool is_working();

private:
    bool read(int port, SramOp *op);
    bool write(int port, SramOp *op);

    int addr_to_line_index(mem_addr addr);
    int size_to_line_num(mem_size size);

    bool check_addr(mem_addr addr);
    bool check_size(mem_size size);

    bool check_valid(mem_addr addr, mem_size size);
    bool check_read(mem_addr addr, mem_size size);
    bool check_write(mem_addr addr, mem_size size);

    void set_valid(mem_addr addr, mem_size size);
    void reset_valid(mem_addr addr, mem_size size);

    SramOpReg requests;

    std::string name;
    int line_size;
    int n_lines;
    int bit_width;
    int n_rw_ports;
    int cycles_per_access;
    
    SramPort *ports;
    SramLine *lines;
    
    long n_reads;
    long n_writes;
};

#endif
