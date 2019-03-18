#include "sram.hpp"

using namespace std;

Sram::Sram(const string& name, int line_size, int num_lines, int bit_width,
            int num_read_write_ports, int num_cycle_per_access) {

    this->name = name;
    this->line_size = line_size;
    this->n_lines = num_lines;
    this->bit_width = bit_width;
    this->n_rw_ports = num_read_write_ports;
    this->cycles_per_access = num_cycle_per_access;

    n_reads = 0;
    n_writes = 0;

    ports = new SramPort[n_rw_ports];
    for(int i = 0; i < n_rw_ports; ++i) {
        ports[i].is_busy = false;
        ports[i].cur_access_cycle = 0;
        ports[i].op = NULL;
    }

    lines = new SramLine[n_lines];
    for(int i = 0; i < n_lines; ++i) {
        lines[i].valid = true;
        lines[i].is_partial_sum = true;
    }
}

Sram::~Sram() {
    if (ports) {
        delete [] ports;
    }

    if (lines) {
        delete [] lines;
    }
}

void Sram::tick() {
    bool all_ports_busy = true;

    for(int i = 0; i < n_rw_ports; ++i) {
        if(ports[i].is_busy){
            cout << "SRAM " << name << " port " << i << " is busy." << endl;
            ++ports[i].cur_access_cycle;
            if(ports[i].cur_access_cycle >= cycles_per_access) {
                SramOp *op = ports[i].op;
                cout << "SRAM " << name << " port " << i;
                if(op->is_read) {
                    cout << " READ";
                } else {
                    cout << " WRITE";
                    set_valid(op->addr, op->size);
                }
                cout << " is complete: ";
                cout << " addr = " << op->addr << ", size = " << op->size << "." << endl;
                op->is_complete = true;
                ports[i].is_busy = false;
                ports[i].op = NULL;

                all_ports_busy = false;
            }
        } else {
            all_ports_busy = false;
        }
    }

    if(!requests.empty()) {
        if(!all_ports_busy) {
            for(int i = 0; i < n_rw_ports; ++i) {
                if(!ports[i].is_busy) {
                    SramOp *op = requests.front();
                    requests.pop();
                    if(op->is_read) {
                        assert(read(i, op));
                    } else {
                        assert(write(i, op));
                    }
                    if(requests.empty())
                        break;
                }
            }
        } else {
            SramOp *op = requests.front();
            cout << "Request is pending: ";
            if(op->is_read) {
                cout << "READ ";
            } else {
                cout << "WRITE ";
            }
            cout << "addr = " << op->addr << ", size = " << op->size << "." << endl;
        }
    }
}

void Sram::push_request(SramOp *op) {
    requests.push(op);
}

int Sram::addr_to_line_index(mem_addr addr) {
    return addr / line_size;
}

int Sram::size_to_line_num(mem_size size) {
    return size / line_size;
}

bool Sram::check_addr(mem_addr addr) {
    return addr % line_size;
}

bool Sram::check_size(mem_size size) {
    return size % line_size;
}

bool Sram::check_valid(mem_addr addr, mem_size size) {
    int line_index = addr_to_line_index(addr);
    int line_num = size_to_line_num(size);

    if(line_index >= n_lines) {
        return false;
    }

    for(int i = line_index; i < line_num; ++i) {
        if(!lines[i].valid) {
            return false;
        }
    }
    return true;
}

bool Sram::check_read(mem_addr addr, mem_size size) {
    return check_valid(addr, size);
}

bool Sram::check_write(mem_addr addr, mem_size size) {
    int line_index = addr_to_line_index(addr);
    int line_num = size_to_line_num(size);

    if(line_index >= n_lines) {
        return false;
    }

    for(int i = line_index; i < line_num; ++i) {
        if(!lines[i].is_partial_sum) {
            return false;
        }
    }
    return true;
}

void Sram::set_valid(mem_addr addr, mem_size size) {
    int line_index = addr_to_line_index(addr);
    int line_num = size_to_line_num(size);

    for(int i = line_index; i < line_num; ++i) {
        lines[i].valid = true;
    }
}

void Sram::reset_valid(mem_addr addr, mem_size size) {
    int line_index = addr_to_line_index(addr);
    int line_num = size_to_line_num(size);

    for(int i = line_index; i < line_num; ++i) {
        lines[i].valid = false;
    }
}

bool Sram::read(int port, SramOp *op) {
    mem_addr addr;
    mem_size size;

    if(!check_addr(addr) || !check_size(size) || !check_read(addr, size)) {
        return false;
    }

    ++n_reads;

    std::cout << "SRAM " << name << " port " << port << " READ is sent: ";
    cout << " addr = " << addr << ", size = " << size;

    #if DEBUG
    int line_index = addr_to_line_index(addr);
    int line_num = size_to_line_num(size);
    cout << ", line index = " << line_index << ", line size = " << line_num;
    #endif

    cout << "." << endl;

    ports[port].is_busy = true;
    ports[port].cur_access_cycle = 0;
    ports[port].op = op;

    return true;
}

bool Sram::write(int port, SramOp *op) {
    mem_addr addr;
    mem_size size;

    if(!check_addr(addr) || !check_size(size) || !check_write(addr, size)) {
        return false;
    }

    ++n_writes;

    std::cout << "SRAM " << name << " port " << port << " WRITE is sent: ";
    cout << " addr = " << addr << ", size = " << size;
    
    #if DEBUG
    int line_index = addr_to_line_index(addr);
    int line_num = size_to_line_num(size);
    cout << ", line index = " << line_index << ", line size = " << line_num;
    #endif

    cout << "." << endl;

    // For Consistency Requirements
    reset_valid(addr, size);

    ports[port].is_busy = true;
    ports[port].cur_access_cycle = 0;
    ports[port].op = op;

    return true;
}
