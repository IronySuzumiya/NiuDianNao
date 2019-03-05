#include "control_processor.hpp"

using namespace std;

ControlProcessor::ControlProcessor(DnnConfig *cfg, Datapath *dp) {
    this->cfg = cfg;
    this->dp = dp;
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

bool ControlProcessor::read_instructions(istream& is) {
    while(!is.eof()) {
        ControlInstruction ins;
        is >> ins;
        ins.state = ControlInstruction::BEGIN;
        ciq.push(ins);
    }
}

bool ControlProcessor::execute_instruction(ControlInstruction *ci) {
    bool done = false;

    if(ci->state == ControlInstruction::BEGIN) {
        assert(ci->sb_read_op == ControlInstruction::LOAD);
        if(ci->nbin_read_op == ControlInstruction::LOAD) {
            ci->state = ControlInstruction::LOAD_NBIN;
        } else {
            assert(ci->nbin_read_op == ControlInstruction::READ);
            ci->state = ControlInstruction::LOAD_SB;
        }
        if(ci->nbout_read_op == ControlInstruction::READ) {
            // do something
        } else {
            assert(ci->nbout_read_op == ControlInstruction::NOP);
            // do something
        }
        if(ci->nbout_write_op == ControlInstruction::WRITE) {
            nbout_store = false;
        } else {
            assert(ci->nbout_write_op == ControlInstruction::STORE);
            nbout_store = true;
        }
        assert(ci->nfu_nfu1_op == ControlInstruction::MULT);
        if(ci->nfu_nfu2_op == ControlInstruction::ADD) {
            // customize the datapath
        } else {
            assert(ci->nfu_nfu2_op == ControlInstruction::MAX);
            // customize the datapath
        }
        if(ci->nfu_nfu2_in == ControlInstruction::RESET) {
            // customize the datapath
        } else {
            assert(ci->nfu_nfu2_in == ControlInstruction::NBOUT);
            // customize the datapath
        }
        if(ci->nfu_nfu2_out == ControlInstruction::NBOUT) {
            // customize the datapath
        } else {
            assert(ci->nfu_nfu2_out == ControlInstruction::NFU3);
            // customize the datapath
        }
        assert(ci->nfu_nfu3_op == ControlInstruction::SIGMOID);
    }

    switch(ci->state){
        case ControlInstruction::LOAD_NBIN:
            cout << "Load NBin: ADDR = " << ci->nbin_address
                << " SIZE = " << ci->nbin_size << endl;
            dp->load_nbin(ci->nbin_address, 0, ci->nbin_size);
            ci->state = ControlInstruction::LOAD_SB;
            break;
        
        case ControlInstruction::LOAD_SB:
            cout << "Load SB: ADDR = " << ci->sb_address
                << " SIZE = " << ci->sb_size << endl;
            dp->load_sb(ci->sb_address, 0, ci->sb_size);
            ci->state = ControlInstruction::DO_OP;
            sb_index = 0;
            break;

        case ControlInstruction::DO_OP:
            if(dp->is_all_needed_data_in_sram()) {
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
                        ci->state = ControlInstruction::STORE_NBOUT;
                    } else {
                        done = true;
                    }
                }
            }
            break;

        case ControlInstruction::STORE_NBOUT:
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