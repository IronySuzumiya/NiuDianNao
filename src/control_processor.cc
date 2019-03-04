#include "control_processor.hh"

ControlProcessor::ControlProcessor(DnnConfig *cfg, Datapath *dp) {
    read_callback = new DRAMSim::Callback<ControlProcessor, void, unsigned, uint64_t, uint64_t>
                            (this, &ControlProcessor::read_complete_callback);
    write_callback = new DRAMSim::Callback<ControlProcessor, void, unsigned, uint64_t, uint64_t>
                            (this, &ControlProcessor::write_complete_callback);
}