#include "control_processor.hh"

using namespace std;

ControlProcessor::ControlProcessor(DnnConfig *cfg, Datapath *dp, Dram *dram) {
    this->cfg = cfg;
    this->dp = dp;
    this->dram = dram;

    read_callback = new DRAMSim::Callback<ControlProcessor, void, unsigned, uint64_t, uint64_t>
                            (this, &ControlProcessor::read_complete_callback);
    write_callback = new DRAMSim::Callback<ControlProcessor, void, unsigned, uint64_t, uint64_t>
                            (this, &ControlProcessor::write_complete_callback);
    dram->set_callbacks(m_read_callback, m_write_callback);
}

void ControlProcessor::tick() {
    if(!ciq.empty()) {
        ControlInstruction *ci = &ciq.front();
        if(execute_instruction(ci)) {
            ciq.pop();
        }
    }
}

bool read_instructions(ifstream& is) {
    // TODO
    return false;
}

bool read_instructions(istringstream& is) {
    while(!is.eof()) {
        ControlInstruction ins;
        is >> ins;
        ins.state = ControlInstruction::ci_state::LOAD_SB;
        ciq.push(ins);
    }
}

void read_complete_callback(unsigned id, uint64_t address, uint64_t clock_cycle) {
    std::cout << "DRAM Read callback for address "  <<  address << " (cycle: " << clock_cycle << ")" << std::endl;

    DramOpReg::iterator it;
    for(it = requests.begin(); it != requests.end(); ++it) {
        if(addr == (*it)->addr) {
            (*it)->is_complete = true;
            break;
        }
    }
}

void write_complete_callback(unsigned id, uint64_t address, uint64_t clock_cycle) {
    std::cout << "DRAM Write callback for address "  <<  address << " (cycle: " << clock_cycle << ")" << std::endl;

    DramOpReg::iterator it;
    for(it = requests.begin(); it != requests.end(); ++it) {
        if(addr == (*it)->addr) {
            (*it)->is_complete = true;
            break;
        }
    }
}