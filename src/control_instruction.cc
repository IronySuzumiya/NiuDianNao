#include "control_instruction.hh"

std::ostream& operator<<(std::ostream& oss, const ControlInstruction::ci_op op) {
  switch (op) {
    case ControlInstruction::NOP:       oss << "NOP"; break;
    case ControlInstruction::LOAD:      oss << "LOAD"; break;
    case ControlInstruction::STORE:     oss << "STORE"; break;
    case ControlInstruction::READ:      oss << "READ"; break;
    case ControlInstruction::WRITE:     oss << "WRITE"; break;
    case ControlInstruction::MULT:      oss << "MULT"; break;
    case ControlInstruction::ADD:       oss << "ADD"; break;
    case ControlInstruction::RESET:     oss << "RESET"; break;
    case ControlInstruction::NBOUT:     oss << "NBOUT"; break;
    case ControlInstruction::NFU3:      oss << "NFU3"; break;
    case ControlInstruction::SIGMOID:   oss << "SIGMOID"; break;
    case ControlInstruction::INVALID:   oss << "INVALID"; break;
    default:                            oss << "UNDEFINED"; break;
  }
  return oss;
}

std::istream& operator>>(std::istream &is, ControlInstruction::ci_op& op) {
  std::string str;
  is >> str;
  if      (str == "NOP")        op = ControlInstruction::NOP;
  else if (str == "LOAD")       op = ControlInstruction::LOAD;
  else if (str == "STORE")      op = ControlInstruction::STORE;
  else if (str == "READ")       op = ControlInstruction::READ;
  else if (str == "WRITE")      op = ControlInstruction::WRITE;
  else if (str == "MULT")       op = ControlInstruction::MULT;
  else if (str == "ADD")        op = ControlInstruction::ADD;
  else if (str == "RESET")      op = ControlInstruction::RESET;
  else if (str == "NBOUT")      op = ControlInstruction::NBOUT;
  else if (str == "NFU3")       op = ControlInstruction::NFU3;
  else if (str == "SIGMOID")    op = ControlInstruction::SIGMOID;
  else                          op = ControlInstruction::INVALID;
  return is;
}

std::ostream& operator<<(std::ostream& oss, const ControlInstruction& ins) {
  oss << " | "  << ins.cp_end;
  oss << " || " << ins.sb_read_op;
  oss << " | "  << ins.sb_reuse;
  oss << " | "  << ins.sb_address;
  oss << " | "  << ins.sb_size;
  oss << " || " << ins.nbin_read_op;
  oss << " | "  << ins.nbin_reuse;
  oss << " | "  << ins.nbin_stride;
  oss << " | "  << ins.nbin_stride_begin;
  oss << " | "  << ins.nbin_stride_end;
  oss << " | "  << ins.nbin_address;
  oss << " | "  << ins.nbin_size;
  oss << " || " << ins.nbout_read_op;
  oss << " | "  << ins.nbout_write_op;
  oss << " | "  << ins.nbout_address;
  oss << " | "  << ins.nbout_size;
  oss << " || " << ins.nfu_nfu1_op;
  oss << " | "  << ins.nfu_nfu2_op;
  oss << " | "  << ins.nfu_nfu2_in;
  oss << " | "  << ins.nfu_nfu2_out;
  oss << " | "  << ins.nfu_nfu3_op;
  oss << " | "  << ins.nfu_output_begin;
  oss << " | "  << ins.nfu_output_end;
  oss << " | ";
}

std::istream & operator>>( std::istream& is, ControlInstruction& ins ) {
  std::string dummy;
  is >> dummy >> ins.cp_end;            
  is >> dummy >> ins.sb_read_op;
  is >> dummy >> ins.sb_reuse;
  is >> dummy >> ins.sb_address;
  is >> dummy >> ins.sb_size;
  is >> dummy >> ins.nbin_read_op;
  is >> dummy >> ins.nbin_reuse;
  is >> dummy >> ins.nbin_stride;
  is >> dummy >> ins.nbin_stride_begin;
  is >> dummy >> ins.nbin_stride_end;
  is >> dummy >> ins.nbin_address;
  is >> dummy >> ins.nbin_size;
  is >> dummy >> ins.nbout_read_op;
  is >> dummy >> ins.nbout_write_op;
  is >> dummy >> ins.nbout_address;
  is >> dummy >> ins.nbout_size;
  is >> dummy >> ins.nfu_nfu1_op;
  is >> dummy >> ins.nfu_nfu2_op;
  is >> dummy >> ins.nfu_nfu2_in;
  is >> dummy >> ins.nfu_nfu2_out;
  is >> dummy >> ins.nfu_nfu3_op;
  is >> dummy >> ins.nfu_output_begin;
  is >> dummy >> ins.nfu_output_end;
  is >> dummy;
  return is;
}  
