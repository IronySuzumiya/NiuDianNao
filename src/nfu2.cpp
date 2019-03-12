#include"nfu2.hpp"

using namespace std;

PipeStageNFU2::PipeStageNFU2(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, 
int n_stages,  unsigned num_adders, unsigned num_shifters, unsigned num_max): 
PipeStage(reg_in, reg_out, queue_size, n_stages),
m_num_adders(num_adders), m_num_shifters(num_shifters), m_num_max(num_max){

   m_adders = new functional_unit*[m_num_adders];
    for(unsigned i=0; i<m_num_adders; ++i){
        m_adders[i] = new functional_unit();
    }

    m_shifters = new functional_unit*[m_num_shifters];
    for(unsigned i=0; i<m_num_shifters; ++i){
        m_shifters[i] = new functional_unit();
    }
    
    m_max = new functional_unit*[m_num_max];
    for(unsigned i=0; i<m_num_max; ++i){
        m_max[i] = new functional_unit();
    }
    
}

PipeStageNFU2::~PipeStageNFU2() {
    if(m_adders){
        for(unsigned i=0; i<m_num_adders; ++i){
            if(m_adders[i])
                delete m_adders[i];
        }
        delete[] m_adders;
    }
    if(m_shifters){
        for(unsigned i=0; i<m_num_shifters; ++i){
            if(m_shifters[i])
                delete m_shifters[i];
        }
        delete[] m_shifters;
    }
    if(m_max){
        for(unsigned i=0; i<m_num_max; ++i){
            if(m_max[i])
                delete m_max[i];
        }
        delete[] m_max;
    }
}

void PipeStageNFU2::tick() {
    PipeOp *op;
    if (!is_pipe_op_reg_full(reg_out)) {
        op = pipeline[n_stages];
        if(op) {
            reg_out->push(op);

            for(unsigned i = 0; i < m_num_adders; i++)
            {
                m_adders[i]->do_op();
            }
            for(unsigned i = 0; i < m_num_shifters; i++)
            {
                m_num_shifters[i]->do_op();
            }
            for(unsigned i = 0; i < m_num_max; i++)
            {
                m_max[i]->do_op();
            }
        }

        for(int i = n_stages; i > 0; i--)
        {
            pipeline[i] = pipeline[i-1];
        }
        pipeline[0] = NULL;

        if (!reg_in->empty()) {
            cout <<"NFU-2 has input, not empty" << endl;
            op = reg_in->front();
            reg_in->pop();
            pipeline[0] = op;
        }   
    }
    else {
      cout << "nfu-2 has no space in the pipeline" << endl; 
    }
}

void PipeStageNFU2::print() {
    cout << "NFU-2: ";
    PipeStage::print();

}