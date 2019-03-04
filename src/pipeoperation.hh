#ifndef __PIPE_OPERATION__
#define __PIPE_OPERATION__

#include "common.hh"
#include "Sram.hh"

typedef std::queue<PipeOp *> PipeOpReg;

class PipeOp {
public:
    PipeOp(mem_addr nbin_addr, mem_size nbin_size,
            mem_addr sb_addr, mem_size sb_size,
            mem_addr nbout_addr, mem_size nbout_size,
            int serial_num);
    ~PipeOp() {}

    bool is_ready();
    
    bool is_read();
    bool is_read_complete();
    bool is_write_complete();

    void set_read();
    void set_write();
    
    bool is_in_pipe_reg();
    void set_in_pipe_reg();
    
    int get_serial_num();
    
private:
    bool _is_in_pipe_reg;  //it indicates if the op has been already moved from the request queue to the pipeline queue
    bool _is_read; // areant all ops reads?
    SramOp nbin_op;
    SramOp sb_op;
    SramOp nbout_op;
    int serial_num;
};

#endif
