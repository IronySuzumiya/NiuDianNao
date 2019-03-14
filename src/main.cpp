#include "niudiannao.hpp"

static int sg_argc = 3;
static const char *sg_argv[] = {"", "-config", "config"};

int main(int argc, char** argv) {
    DnnConfig *dnn_config = new DnnConfig();
    
    option_parser_t opp = option_parser_create();
    dnn_config->reg_options(opp);
    option_parser_cmdline(opp, sg_argc, sg_argv);
    option_parser_print(opp, stdout);

    NiuDianNao *ndn = new NiuDianNao(dnn_config);
    ndn->run();

    delete ndn;
    
    return 0;
}
