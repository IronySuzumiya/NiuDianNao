#include "niudiannao.hpp"

using namespace std;

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
    cp->read_instructions("| NOP || LOAD | 0 | 0 | 32768 "
        "|| LOAD | 1 | 0 | 0 | 0 | 4194304 | 2048 "
        "|| NOP | WRITE | 0 | 0 || MULT | ADD | RESET | NBOUT | SIGMOID | 1 | 0 |");
    for(;;) {
        tick();
        if(!cp->is_working() && !dp->is_working()) {
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
