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
            cout << "Done" << endl;
            ciq.pop();
        }
    }
}

bool ControlProcessor::read_instructions(ifstream& is) {
    // TODO
    return false;
}

bool ControlProcessor::read_instructions(istringstream& is) {
    while(!is.eof()) {
        ControlInstruction ins;
        is >> ins;
        ins.state = BEGIN;
        ciq.push(ins);
    }
}

bool ControlProcessor::execute_instruction(ControlInstruction *ci) {
    bool done = false;

    if(ci->state == BEGIN) {
        assert(ci->sb_read_op == LOAD);
        if(ci->nbin_read_op == LOAD) {
            ci->state = LOAD_NBIN;
        } else {
            assert(ci->nbin_read_op == READ);
            ci->state = LOAD_SB;
        }
        if(ci->nbout_read_op == READ) {
            // do something
        } else {
            assert(ci->nbout_read_op == NOP);
            // do something
        }
        if(ci->nbout_write_op == WRITE) {
            nbout_store = false;
        } else {
            assert(ci->nbout_write_op == STORE);
            nbout_store = true;
        }
        assert(nfu_nfu1_op == MULT);
        if(nfu_nfu2_op == ADD) {
            // customize the datapath
        } else {
            assert(nfu_nfu2_op == MAX);
            // customize the datapath
        }
        if(nfu_nfu2_in == RESET) {
            // customize the datapath
        } else {
            assert(nfu_nfu2_in == NBOUT);
            // customize the datapath
        }
        if(nfu_nfu2_out == NBOUT) {
            // customize the datapath
        } else {
            assert(nfu_nfu2_out == NFU3);
            // customize the datapath
        }
        assert(nfu_nfu3_op == SIGMOID);
    }

    switch(ci->state){
        case LOAD_NBIN:
            cout << "Load NBin: ADDR = " << ci->nbin_address
                << " SIZE = " << ci->nbin_size << endl;
            dp->load_nbin(ci->nbin_address, 0, ci->nbin_size);
            ci->state = LOAD_SB;
            break;
        
        case LOAD_SB:
            cout << "Load SB: ADDR = " << ci->sb_address
                << " SIZE = " << ci->sb_size << endl;
            dp->load_sb(ci->sb_address, 0, ci->sb_size);
            ci->state = DO_OP;
            sb_index = 0;
            break;

        case DO_OP:
            if(dp->is_ready()) {
                int data_size = (cfg->bit_width / 8); // in bytes

                int num_output_lines    = cfg->num_outputs / cfg->nbout_line_length;
                int nbin_index          = sb_index / num_output_lines;
                int nbout_index         = sb_index % num_output_lines;

                int sb_addr     = ci->sb_address      + sb_index      * cfg->sb_line_length      * data_size;
                int nbin_addr   = ci->nbin_address    + nbin_index    * cfg->nbin_line_length    * data_size;
                int nbout_addr  = ci->nbout_address   + nbout_index   * cfg->nbout_line_length   * data_size;

                cout << "Do Op: NBIN ADDR = " << nbin_addr
                    << " SB ADDR = " << sb_addr
                    << " NBOUT ADDR = " << nbout_addr << endl;

                // not right
                PipeOp *op = new PipeOp(nbin_addr, 1, sb_addr, 1, nbout_addr, 1);
                dp->push_pipe_op(op);
                ++sb_index;

                if (sb_index == cfg->sb_num_lines) {
                    if(nbout_store) {
                        ci->state = STORE_NBOUT;
                    } else {
                        done = true;
                    }
                }
            }
            break;

        case STORE_NBOUT:
            cout << "Store NBout: ADDR = " << ci->nbout_address
                << " SIZE = " << ci->nbout_size << endl;
            dp->store_nbout(0, ci->nbout_address, ci->nbout_size);
            done = true;
            break;

        default:
            cout << "Error: Undefined instruction state" << endl;
            abort();
    }
    return done;
}