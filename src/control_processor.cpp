#include "control_processor.hpp"

using namespace std;

void ControlProcessor::tick() {
    if(!ciq.empty()) {
        ControlInstruction *ci = &ciq.front();
        if(execute_instruction(ci)) {
            cout << "Control Instruction is Done:" << endl;
            cout << *ci << endl;
            ciq.pop();
        }
    }
}

void ControlProcessor::read_instructions(string s) {
    stringstream is(s);
    read_instructions(is);
}

void ControlProcessor::read_instructions(istream& is) {
    while(!is.eof()) {
        ControlInstruction ins;
        is >> ins;
        ins.state = ControlInstruction::BEGIN;
        cout << "Read Control Instruction:" << endl;
        cout << ins << endl;
        ciq.push(ins);
    }
}

bool ControlProcessor::is_working() {
    return !ciq.empty();
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
            assert(ci->nfu_nfu2_out == ControlInstruction::NFU3);
        } else {
            assert(ci->nbout_read_op == ControlInstruction::NOP);
            assert(ci->nfu_nfu2_out == ControlInstruction::NBOUT);
        }
        assert(ci->nfu_nfu1_op == ControlInstruction::MULT);
        assert(ci->nfu_nfu3_op == ControlInstruction::SIGMOID);
        cout << "Current Control Instruction:" << endl;
        cout << *ci << endl;
    }

state_machine:
    switch(ci->state) {
        case ControlInstruction::LOAD_NBIN:
            cout << "Load NBin: addr = " << ci->nbin_address
                << ", size = " << ci->nbin_size << "." << endl;
            dp->load_nbin(ci->nbin_address, 0, ci->nbin_size);
            ci->state = ControlInstruction::LOAD_SB;
            break;
        
        case ControlInstruction::LOAD_SB:
            cout << "Load SB: addr = " << ci->sb_address
                << ", size = " << ci->sb_size << "." << endl;
            dp->load_sb(ci->sb_address, 0, ci->sb_size);
            ci->state = ControlInstruction::CUSTOMIZE;
            sb_index = 0;
            break;

        case ControlInstruction::CUSTOMIZE:
            if(!dp->is_ready()) {
                break;
            }
            // In practice, this state should not occupy one process cycle.
            // But since datapath is waiting for DRAM access at this moment,
            // this won't hurt.
            cout << "Customize Datapath." << endl;
            if(ci->nbout_write_op == ControlInstruction::WRITE) {
                nbout_store = false;
            } else {
                assert(ci->nbout_write_op == ControlInstruction::STORE);
                nbout_store = true;
            }
            if(ci->nfu_nfu2_op == ControlInstruction::ADD) {
                dp->switch_nfu2_to_add_mode();
            } else {
                assert(ci->nfu_nfu2_op == ControlInstruction::MAX);
                dp->switch_nfu2_to_max_mode();
            }
            if(ci->nfu_nfu2_in == ControlInstruction::RESET) {
                dp->nfu2_read_reset();
            } else {
                assert(ci->nfu_nfu2_in == ControlInstruction::NBOUT);
                dp->nfu2_read_nbout();
            }
            if(ci->nfu_nfu2_out == ControlInstruction::NBOUT) {
                dp->deactivate_nfu3();
            } else {
                assert(ci->nfu_nfu2_out == ControlInstruction::NFU3);
                dp->activate_nfu3();
            }
            ci->state = ControlInstruction::DO_OP;
            goto state_machine;
            break;

        case ControlInstruction::DO_OP:
            {
                // FIXME:
                //
                // /-----------------------------------\
                // | W0,0     W0,1    ...     W0,15    |  ...  W0,255
                // | ...      ...     ...     ...      |  ...  ...
                // | W1023,0  W1023,1 ...     W1023,15 |  ...  W1023,255
                // \-----------------------------------/
                //   ...      ...     ...     ...         ...  ...
                //   W8191,0  W8191,1 ...     W8191,15    ...  W8191,255
                //
                // /-----------------------------------\
                // | W0,0     W0,1    ...     W0,15    |
                // | W1,0     W1,1    ...     W1,15    |
                // | ...      ...     ...     ...      |
                // | W15,0    W15,1   ...     W15,15   |
                // \-----------------------------------/

                int data_size = (cfg->bit_width / 8); // in bytes

                int num_output_lines    = cfg->num_outputs / cfg->nbout_line_length;
                int nbin_index          = sb_index / num_output_lines;
                int nbout_index         = sb_index % num_output_lines;

                int sb_addr     = sb_index      * cfg->sb_line_length      * data_size;
                int nbin_addr   = nbin_index    * cfg->nbin_line_length    * data_size;
                int nbout_addr  = nbout_index   * cfg->nbout_line_length   * data_size;

                cout << "Next Op: NBin addr = " << nbin_addr
                    << ", SB addr = " << sb_addr
                    << ", NBout addr = " << nbout_addr << "." << endl;
                // FIXME: 'is_partial' should not be '!nbout_store'
                PipeOp *op = new PipeOp(nbin_addr, 16 * 2, sb_addr, 256 * 2, nbout_addr, 16 * 2, !nbout_store);
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
            if(!dp->is_working()) {
                cout << "Store NBout: addr = " << ci->nbout_address
                    << ", size = " << ci->nbout_size << endl;
                assert(dp->can_write_back());
                dp->store_nbout(ci->nbout_address, 0, ci->nbout_size);
                done = true;
            }
            break;

        default:
            cout << "Error: Undefined instruction state" << endl;
            abort();
    }
    return done;
}