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
        ins.state = ControlInstruction::ci_state::LOAD_SB;
        ciq.push(ins);
    }
}

bool ControlProcessor::execute_instruction(ControlInstruction *ci) {
    bool done = false;

    // All these states should be pipelined, we want to start computing once 
    // the first buffer entries are loaded
    switch(ci->state){
        case cp_inst::LOAD_SB: // Load from DRAM into the SB SRAM
            std::cout << "LOAD_SB " << inst->sb_address << std::endl;
            if(m_dram_interface->can_accept_request()){
                mf = new memory_fetch(inst->sb_address, inst->sb_size, READ, SB);

                // TODO: This is only going to get one part of the data. The control processor will need to
                // issue multiple DRAM read requests to populate the NBin and SB SRAMs
                // m_dram_interface->do_access(mf);
                m_dram_interface->push_request(mf->m_addr, false);
               
                if(inst->nbin_read_op == cp_inst::LOAD){
                    inst->m_state = cp_inst::LOAD_NBIN;
                }else{
                    inst->m_state = cp_inst::DO_OP;
                }

                //mf->m_is_complete = true; // HACH for TESTING
                m_mem_requests.push_back(mf); // Add memory fetch to pending queue

                m_sb_index = 0;
            }
            break;

        case cp_inst::LOAD_NBIN: // Load from DRAM into the NBin SRAM
            std::cout << "LOAD_NBIN " << inst->nbin_address << std::endl;
            if(m_dram_interface->can_accept_request()){
                mf = new memory_fetch(inst->nbin_address, inst->nbin_size, READ, NBin);

                // TODO: This is only going to get one part of the data. The control processor will need to
                // issue multiple DRAM read requests to populate the NBin and SB SRAMs
                // m_dram_interface->do_access(mf);
                m_dram_interface->push_request(mf->m_addr, false);

                inst->m_state = cp_inst::DO_OP;

               // mf->m_is_complete = true; // HACK for TESTING
                m_mem_requests.push_back(mf); // Add memory fetch to pending queue
            }
            break;

        case cp_inst::DO_OP: // All data is loaded into the SRAMs, push pipe_ops into the main dnn_sim pipeline

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