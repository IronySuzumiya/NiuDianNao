#ifndef __SRAM__
#define __SRAM__

#include "pipe_operation.hh"

typedef struct sram_line {
    bool valid;       // ready to be read
    bool ready_to_writeback;    // ready to be stored to DRAM
    bool is_partial_sum; // partial sum, can be overwritten
    mem_addr addr;    // for sanity check, we shouldn't need to store this in hardware
} SramLine;

typedef struct sram_port {
    bool is_busy;
    int cur_access_cycle;
    SramOp *op;
} SramPort;

typedef struct sram_op {
    mem_addr addr;
    mem_size size;
    bool is_read;
    bool is_complete;
} SramOp;

typedef std::queue<SramOp *> SramOpReg;

class Sram {
public:
    Sram(const std::string& name, int line_size, int num_lines, int bit_width,
            int num_read_write_ports, int num_cycle_per_access);
    ~Sram();
    
    void tick();
    bool is_busy();
    void set_valid();
    void reset_valid();

    void push_request(SramOp *op);

private:
    bool read(int port, SramOp *op);
    bool write(int port, SramOp *op);

    bool check_addr(mem_addr addr);
    bool check_valid(mem_addr addr);
    bool check_write(mem_addr addr);

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
