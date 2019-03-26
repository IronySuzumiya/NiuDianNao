#include "niudiannao.hpp"

using namespace std;

static const char instfile[] = "inst";

NiuDianNao::NiuDianNao(DnnConfig *cfg) {
    dp = new Datapath(cfg);
    cp = new ControlProcessor(cfg, dp);
    cycle = 0;
}

NiuDianNao::~NiuDianNao() {
    delete dp;
    delete cp;
}

void NiuDianNao::run() {
    ifstream is(instfile);
    cp->read_instructions(is);
    for(;;) {
        tick();
        if(!cp->is_working() && !dp->is_working() && !dp->is_dram_working()) {
            break;
        }
    }
}

void NiuDianNao::tick() {
    ++cycle;
    cout << endl << "Cycle: " << cycle << endl;
    dp->print_pipeline();
    dp->tick();
    cp->tick();
}
