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
            // do something about it
        } else {
            assert(ci->nbout_read_op == NOP);
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

    // All these states should be pipelined, we want to start computing once 
    // the first buffer entries are loaded
    switch(ci->state){
        case LOAD_NBIN: // Load from DRAM into the NBin SRAM
            std::cout << "Load NBin: ADDR = " << ci->nbin_address
                << " SIZE = " << ci->nbin_size << std::endl;
            dp->load_nbin(ci->nbin_address, 0, ci->nbin_size);
            ci->state = LOAD_SB;
            break;
        
        case cp_inst::LOAD_SB: // Load from DRAM into the SB SRAM
            std::cout << "Load SB: ADDR = " << ci->sb_address
                << " SIZE = " << ci->sb_size << std::endl;
            dp->load_sb(ci->sb_address, 0, ci->sb_size);
            ci->state = DO_OP;
            sb_index = 0;
            break;

        case cp_inst::DO_OP: // All data is loaded into the SRAMs, push pipe_ops into the main dnn_sim pipeline

            
            #if 0
            // First wait for all loads to complete, write data to SRAMs
            if(m_mem_requests.size() > 0){
                memory_fetch *mf = m_mem_requests.front();

                if(mf->m_is_complete){
                    // Write the data to the SRAM
                    if(m_datapath->write_sram(mf->m_addr, mf->m_size, mf->m_sram_type)){
                        // Write went through, pop the request from the mem_req queue
                        m_mem_requests.pop_front();
                        if(m_mem_requests.size() > 0)
                            pending_req = true;
                    }else {
                        // Otherwise, all SRAM ports were busy, try again next cycle
                        return false;
                    }

                }else{
                    pending_req = true;
                    std::cout << "DO_OP waiting for pending request\n";
                }
            }

            // Then start doing the main operation if no pending DRAM READS
            // Patrick: Can't we start processing data while the buffers are being filled?
            if(!pending_req) {
                // This is where I would start creating "pipe_ops" to perform the convolution, cycling through the different filters loaded into SB
                 std::cout << "Start proccessing pipeline" << std::endl; 
                int data_size = (m_dnn_config->bit_width / 8); // in bytes

                int num_output_lines = m_dnn_config->num_outputs / m_dnn_config->nbout_line_length; // 16 = 256 / 16
                int nbin_index      = m_sb_index / num_output_lines; 
                int nbout_index     = m_sb_index % num_output_lines; 

                int sb_addr     = inst->sb_address      + m_sb_index    * m_dnn_config->sb_line_length      * data_size;
                int nbin_addr   = inst->nbin_address    + nbin_index    * m_dnn_config->nbin_line_length    * data_size;
                int nbout_addr  = inst->nbout_address   + nbout_index   * m_dnn_config->nbout_line_length   * data_size;

                std::cout << "DO_OP " << nbin_addr << " , " << sb_addr << " , " << nbout_addr << std::endl;
                pipe_op * op = new pipe_op( nbin_addr, 1, sb_addr, 1, nbout_addr, 1, m_serial_counter);
                // insert pipe_op into datapath
                bool was_inserted = m_datapath->insert_op(op);

                if (!was_inserted) {
                    delete op;
                    break;
                }

                m_sb_index++;
                m_serial_counter++;

#ifdef TEST
                // Temporarily end test after DRAM reads complete and pipeline starts
                done = true;
                is_test_complete = true;
#endif

                // should go to STORE_NBOUT first
                if (m_sb_index == m_dnn_config->sb_num_lines) {
                    done = true;
                    std::cout << "Done\n";
                }
            }

            break;
            #endif

        case cp_inst::STORE_NBOUT:
            // Write out NBout to DRAM
            std::cout << "STORE_NBOUT not implemented" << std::endl;
            break;

        default:
            std::cout << "Error: Undefined instruction state. Aborting" << std::endl;
            abort();
    }
    return done;
}