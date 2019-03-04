#ifndef __SRAM__
#define __SRAM__

#include "pipeoperation.hh"

typedef struct sram_line {
    bool valid;       // ready to be read
    bool ready_to_be_writeback;    // ready to be stored to DRAM
    bool can_be_overwritten; // partial sum, can be overwritten
    mem_addr addr;    // for sanity check, we shouldn't need to store this in hardware
} SramLine;

typedef struct sram_port {
    bool is_busy;
    bool is_read;
    int cur_access_cycle;
    SramOp *op;
} SramPort;

struct SramOp {
    mem_addr addr;
    mem_size size;
    bool is_read;
    bool is_complete;
};

typedef std::queue<SramOp *> SramOpReg;

class Sram {
public:
    Sram(const std::string& name, int line_size, int num_lines, int bit_width,
            int num_read_write_ports, int num_cycle_per_access);
    ~Sram();
    
    void tick();
    bool is_busy();

    bool read(int port, SramOp *op);
    bool write(int port, SramOp *op);

private:
    bool check_addr(mem_addr addr);

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
