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
    cp->read_instruction("| NOP || LOAD | 0 | 0 | 32768 "
        "|| LOAD | 1 | 0 | 0 | 0 | 4194304 | 2048 "
        "|| NOP | WRITE | 0 | 0 || MULT | ADD | RESET | NBOUT | SIGMOID | 1 | 0 |");
    for(int i = 0; i < 100; ++i) {
        tick();
    }
    dp->print_stats();
}

void NiuDianNao::tick() {
    dp->tick();
    cp->tick();
    ++cycle;
    cout << "Cycle: " << cycle << endl;
    dp->print_pipeline();
}
