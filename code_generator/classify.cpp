////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// Siu Pak Mok
// 2015
// classify.cpp
// A simple test program
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "code_generator.h"

static const char argfile[] = "args";
static const char outfile[] = "inst";

int main(int argc, char **argv){
    /*if (argc != 12) {
        std::cout << "Found " << argc-1 << " arguments" << std::endl;
        std::cout << "Order of Arguments:" << std::endl;
        std::cout << "num_input_neurons" << std::endl;
        std::cout << "num_output_neurons" << std::endl;
        std::cout << "num_input_neurons_per_entry" << std::endl;
        std::cout << "num_output_neurons_per_entry" << std::endl;
        std::cout << "num_sb_entries" << std::endl;
        std::cout << "num_nbin_entries" << std::endl;
        std::cout << "num_nbout_entries" << std::endl;
        std::cout << "bit_width" << std::endl;
        std::cout << "sb_addr" << std::endl;
        std::cout << "nbin_addr" << std::endl;
        std::cout << "nbout_addr" << std::endl;

        return 4;
    }*/

    std::ifstream is(argfile);

    unsigned num_input_neurons;
    unsigned num_output_neurons;
    unsigned num_input_neurons_per_entry;
    unsigned num_output_neurons_per_entry;
    unsigned num_sb_entries;
    unsigned num_nbin_entries;
    unsigned num_nbout_entries;
    unsigned bit_width;
    unsigned sb_addr;
    unsigned nbin_addr;
    unsigned nbout_addr;

    assert(is >> num_input_neurons);
    assert(is >> num_output_neurons);
    assert(is >> num_input_neurons_per_entry);
    assert(is >> num_output_neurons_per_entry);
    assert(is >> num_sb_entries);
    assert(is >> num_nbin_entries);
    assert(is >> num_nbout_entries);
    assert(is >> bit_width);
    assert(is >> sb_addr);
    assert(is >> nbin_addr);
    assert(is >> nbout_addr);

    std::cout << "num_input_neurons: " << num_input_neurons << std::endl;
    std::cout << "num_output_neurons: " << num_output_neurons << std::endl;
    std::cout << "num_input_neurons_per_entry: " << num_input_neurons_per_entry << std::endl;
    std::cout << "num_output_neurons_per_entry: " << num_output_neurons_per_entry << std::endl;
    std::cout << "num_sb_entries: " << num_sb_entries << std::endl;
    std::cout << "num_nbin_entries: " << num_nbin_entries << std::endl;
    std::cout << "num_nbout_entries: " << num_nbout_entries << std::endl;
    std::cout << "bit_width: " << bit_width << std::endl;
    std::cout << "sb_addr: " << sb_addr << std::endl;
    std::cout << "nbin_addr: " << nbin_addr << std::endl;
    std::cout << "nbout_addr: " << nbout_addr << std::endl;

    std::ofstream os(outfile);

    std::string entry = generate_classify_layer_code ( num_input_neurons,
                                                        num_output_neurons,
                                                        num_input_neurons_per_entry,
                                                        num_output_neurons_per_entry,
                                                        num_sb_entries,
                                                        num_nbin_entries,
                                                        num_nbout_entries,
                                                        bit_width,
                                                        sb_addr,
                                                        nbin_addr,
                                                        nbout_addr,
                                                        os);

    return 0;
}
