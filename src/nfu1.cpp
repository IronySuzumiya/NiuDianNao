 
 #include"nfu1.hpp"
 using namespace std;


PipeStageNFU1::PipeStageNFU1(PipeOpReg *reg_in, PipeOpReg *reg_out, int queue_size,
  int n_stages, unsigned num_multipliers) : PipeStage(reg_in, reg_out, queue_size, n_stages),
  m_num_multipliers(num_multipliers) {

    m_multipliers = new functional_unit*[m_num_multipliers];
    for(unsigned i=0; i<m_num_multipliers; ++i){
        m_multipliers[i] = new functional_unit();
    }
    
 }


PipeStageNFU1::PipeStageNFU1(PipeOpReg *reg_in, PipeOpReg *reg_out, PipeOpReg *reg_requests, Sram * nbin, Sram * sb, 
   int queue_size, int n_stages, unsigned num_multipliers) : PipeStage(reg_in, reg_out, queue_size, n_stages),
   m_num_multipliers(num_multipliers), m_requests(reg_requests){

    
   m_multipliers = new functional_unit*[m_num_multipliers];
    
   for(unsigned i=0; i<m_num_multipliers; ++i){
      m_multipliers[i] = new functional_unit();
   }
    
   nbin = nbin;
   sb = sb;
 }

 PipeStageNFU1::~PipeStageNFU1() {

   if(m_multipliers){
        for(unsigned i=0; i<m_num_multipliers; ++i){
            if(m_multipliers[i])
                delete m_multipliers[i];
        }
        delete[] m_multipliers;
    }

 }

 void PipeStageNFU1::tick() {
    PipeOp *op;
    if (!is_pipe_op_reg_full(reg_out)) {
        op = pipeline[n_stages];
        if(op) {
            cout << "Valid last operation at nfu_1" << endl;
            reg_out->push(op);
            for(unsigned i = 0; i < m_num_multipliers; i++)
            {
                m_multipliers[i]->do_op();
            }
        }
         // Progress internal stage pipeline
        for(int i = n_stages; i > 0; i--)
        {
            pipeline[i] = pipeline[i-1];
        }
        cout << endl;
        pipeline[0] = NULL;

        if (!reg_in->empty()) {
            cout <<"NFU-1 has input, not empty" << endl;
            op = reg_in->front();
             // read sram
            if( nbin->available(op) && sb->available(op) ) {
                nbin->read(op);
                sb->read(op);
                cout << "NFU_1: SRAM reads complete, pushing through" << endl;
                reg_in->pop();
                int_pipeline[0] = op;
            }
        }   
    }
    else {
      cout << "nfu-1 has no space in the pipeline" << endl; 
    }

 }

 void PipeStageNFU1::print() {
    cout << "NFU-1: ";
    PipeStage::print();
 }