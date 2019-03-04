#include "sram.hh"

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
        ports[i].is_read = false;
        ports[i].cur_access_cycle = 0;
        ports[i].op = NULL;
    }

    lines = new SramLine[n_lines];
    for(int i = 0; i < n_lines; ++i) {
        lines[i].valid = true;
    }
}

Sram::~Sram() {
    if (ports)
        delete ports;

    if (lines)
        delete lines;
}

void Sram::tick() {
    bool all_ports_busy = true;
    int port_available;

    for(int i = 0; i < n_rw_ports; ++i){
        if(ports[i].is_busy){
            cout << "SRAM " << name << " port " << i << " is busy." << endl;
            if(ports[i].cur_access_cycle >= cycles_per_access) {
                SramOp *op = ports[i].op;
                ports[i].is_busy = false;
                cout << "SRAM " << name << " port " << i;
                if(ports[i].is_read) {
                    cout << " READ";
                } else {
                    cout << " WRITE";
                }
                cout << " is complete: ";
                cout << " ADDR = " << op->addr << ", SIZE = " << op->size << "." << endl;
                op->is_complete = true;
                ports[i].op = NULL;

                all_ports_busy = false;
            } else {
                ports[i].cur_access_cycle++;
            }
        } else {
            all_ports_busy = false;
            port_available = i;
        }
    }

    if(!requests.empty()) {
        SramOp *op = requests.front();
        if(!all_ports_busy) {
            if(op->is_read) {
                read(port_available, op);
            }else{
                write(port_available, op);
            }
        } else {
            cout << "Request is pending: ";
            if(op->is_read) {
                cout << "READ ";
            } else {
                cout << "WRITE ";
            }
            cout << "ADDR = " << op->addr << ", SIZE = " << op->size << "." << endl;
        }
    }
}

bool Sram::is_busy() {
    for(int i = 0; i < n_rw_ports; ++i) {
        if(!ports[i].is_busy)
            return false;
    }
    return true;
}

bool Sram::check_addr(mem_addr addr) {
    if(addr % (bit_width / 2))
        return false;

    int index = (addr / (bit_width / 2) ) % n_lines;

    return lines[index].valid;
}

// Reads a line from the SRAM array
bool Sram::read(int port, SramOp *op) {
    if(!check_addr(op->addr))
        return false;
    ++n_reads;

    std::cout << "SRAM " << name << " port " << port << " READ is sent: ";
    cout << " ADDR = " << op->addr << " SIZE = " << op->size << "." << endl;

    ports[port].is_busy = true;
    ports[port].is_read = true;
    ports[port].cur_access_cycle = 0;
    ports[port].op = op;

    return true;
}

bool Sram::write(int port, SramOp *op) {
    if(!check_addr(op->addr))
        return false;
    ++n_writes;

    std::cout << "SRAM " << name << " port " << port << " WRITE is sent: ";
    cout << " ADDR = " << op->addr << " SIZE = " << op->size << "." << endl;

    ports[port].is_busy = true;
    ports[port].is_read = false;
    ports[port].cur_access_cycle = 0;
    ports[port].op = op;

    return true;
}
