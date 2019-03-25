#include "sram.hpp"

using namespace std;

#define print_line_info(addr, size) \
do {\
    cout << "line index = " << addr_to_line_index(addr) << ", line size = " << size_to_line_num(size);\
} while(0)

#define check(addr, size, cond, any) \
do {\
    int line_index = addr_to_line_index(addr);\
    int line_num = size_to_line_num(size);\
    if(line_index >= n_lines) {\
        return false;\
    }\
    for(int i = line_index; i < line_num; ++i) {\
        if(any) {\
            check_any(cond);\
        } else {\
            check_not_any(cond);\
        }\
    }\
    return true;\
} while(0)

#define check_any(cond) \
do {\
    if(lines[i].cond) {\
        return false;\
    }\
} while(0)

#define check_not_any(cond) \
do {\
    if(!lines[i].cond) {\
        return false;\
    }\
} while(0)

#define set_field(field, is_set) \
do {\
    int line_index = addr_to_line_index(addr);\
    int line_num = size_to_line_num(size);\
    for(int i = line_index; i < line_num; ++i) {\
        if(is_set) {\
            set(field);\
        } else {\
            reset(field);\
        }\
    }\
} while(0)

#define set(field) lines[i].field = true

#define reset(field) lines[i].field = false

Sram::Sram(const string& name, int line_size, int num_lines, int bit_width,
            int num_read_write_ports, int num_cycle_per_access)
            : name(name), line_size(line_size), n_lines(num_lines),
            bit_width(bit_width), n_rw_ports(num_read_write_ports),
            cycles_per_access(num_cycle_per_access),
            n_reads(0), n_writes(0) {
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
                cout << "addr = " << op->addr << ", size = " << op->size;
                #if DEBUG
                cout << ", ";
                print_line_info(op->addr, op->size);
                #endif
                if(!op->is_read) {
                    if(op->is_partial_sum) {
                        cout << ", partial";
                        set_partial(op->addr, op->size);
                    } else {
                        cout << ", final";
                        set_final(op->addr, op->size);
                    }
                }
                cout << "." << endl;
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
            cout << "addr = " << op->addr << ", size = " << op->size;
            #if DEBUG
            cout << ", ";
            print_line_info(op->addr, op->size);
            #endif
            if(!op->is_read) {
                if(op->is_partial_sum) {
                    cout << ", partial";
                } else {
                    cout << ", final";
                }
            }
            cout << "." << endl;
        }
    }
}

void Sram::push_request(SramOp *op) {
    requests.push(op);
}

bool Sram::is_working() {
    for(int i = 0; i < n_rw_ports; ++i) {
        if(ports[i].is_busy) {
            return true;
        }
    }
    return !requests.empty();
}

bool Sram::check_write_back() {
    // FIXME
    return true;

    //check(0, line_size * n_lines, is_partial_sum, true);
}

int Sram::addr_to_line_index(mem_addr addr) {
    return addr / line_size;
}

int Sram::size_to_line_num(mem_size size) {
    return size / line_size;
}

bool Sram::check_addr(mem_addr addr) {
    return !(addr % line_size);
}

bool Sram::check_size(mem_size size) {
    return !(size % line_size);
}

bool Sram::check_valid(mem_addr addr, mem_size size) {
    check(addr, size, valid, false);
}

bool Sram::check_read(mem_addr addr, mem_size size) {
    return check_valid(addr, size);
}

bool Sram::check_write(mem_addr addr, mem_size size) {
    //FIXME
    return true;

    //check(addr, size, is_partial_sum, false);
}

void Sram::set_valid(mem_addr addr, mem_size size) {
    set_field(valid, true);
}

void Sram::reset_valid(mem_addr addr, mem_size size) {
    set_field(valid, false);
}

void Sram::set_partial(mem_addr addr, mem_size size) {
    set_field(is_partial_sum, true);
}

void Sram::set_final(mem_addr addr, mem_size size) {
    set_field(is_partial_sum, false);
}

bool Sram::read(int port, SramOp *op) {
    mem_addr addr = op->addr;
    mem_size size = op->size;

    if(!check_addr(addr) || !check_size(size) || !check_read(addr, size)) {
        return false;
    }

    ++n_reads;

    std::cout << "SRAM " << name << " port " << port << " READ is sent: ";
    cout << "addr = " << addr << ", size = " << size;

    #if DEBUG
    cout << ", ";
    print_line_info(addr, size);
    #endif

    cout << "." << endl;

    ports[port].is_busy = true;
    ports[port].cur_access_cycle = 0;
    ports[port].op = op;

    return true;
}

bool Sram::write(int port, SramOp *op) {
    mem_addr addr = op->addr;
    mem_size size = op->size;

    if(!check_addr(addr) || !check_size(size) || !check_write(addr, size)) {
        return false;
    }

    ++n_writes;

    std::cout << "SRAM " << name << " port " << port << " WRITE is sent: ";
    cout << "addr = " << addr << ", size = " << size;
    
    #if DEBUG
    cout << ", ";
    print_line_info(addr, size);
    #endif
    if(op->is_partial_sum) {
        cout << ", partial";
    } else {
        cout << ", final";
    }

    cout << "." << endl;

    // For Consistency Requirements
    reset_valid(addr, size);

    ports[port].is_busy = true;
    ports[port].cur_access_cycle = 0;
    ports[port].op = op;

    return true;
}
