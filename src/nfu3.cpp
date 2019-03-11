#include "nfu3.hpp"

using namespace std;

PipeStageNFU3::PipeStageNFU3(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size, 
int n_stages, unsigned num_multipliers, 
unsigned num_adders) : PipeStage(reg_in, reg_out, queue_size, n_stages),
m_num_multipliers(num_multipliers), m_num_adders(num_adders) {

    m_multipliers = new functional_unit*[m_num_multipliers];
    for(unsigned i=0; i<m_num_multipliers; ++i){
        m_multipliers[i] = new functional_unit();
    }
    
    m_adders = new functional_unit*[m_num_adders];
    for(unsigned i=0; i<m_num_adders; ++i){
        m_adders[i] = new functional_unit();
    }

}

PipeStageNFU3::~PipeStageNFU3(){
 if(m_multipliers){
        for(unsigned i=0; i<m_num_multipliers; ++i){
            if(m_multipliers[i])
                delete m_multipliers[i];
        }
        delete[] m_multipliers;
    }
    
    if(m_adders){
        for(unsigned i=0; i<m_num_adders; ++i){
            if(m_adders[i])
                delete m_adders[i];
        }
        delete[] m_adders;
    }
}

void PipeStageNFU3::tick() {

    PipeOp *op;
    if (!is_pipe_op_reg_full(reg_out)) {
        op = pipeline[n_stages];
        if(op) {
            reg_out->push(op);

            for(unsigned i = 0; i < m_num_multipliers; i++)
            {
                m_multipliers[i]->do_op();
            }
            for(unsigned i = 0; i < m_num_adders; i++)
            {
                m_adders[i]->do_op();
            }
        }

        for(int i = n_stages; i > 0; i--)
        {
            pipeline[i] = pipeline[i-1];
        }
        pipeline[0] = NULL;

        if (!reg_in->empty()) {
            cout <<"NFU-3 has input, not empty" << endl;
            op = reg_in->front();
            reg_in->pop();
            op->is_read = true;
            pipeline[0] = op;
        }   
    } 
    else {
      cout << "nfu-3 has no space in the pipeline" << endl; 
    }
  
}

void PipeStageNFU3::print() {
    cout << "NFU-3: ";
    PipeStage::print();
}