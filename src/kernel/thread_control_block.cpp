#include "thread_control_block.hpp"

uint64_t thread_id_counter = 0;

ThreadControlBlock::ThreadControlBlock() {
  this->thread_id = thread_id_counter++;
}

void GeneralPurposeRegisterStates::read_from_sp(uint64_t *base) {
  this->x0 = base[0];
  this->x1 = base[1];
  this->x2 = base[2];
  this->x3 = base[3];
  this->x4 = base[4];
  this->x5 = base[5];
  this->x6 = base[6];
  this->x7 = base[7];
  this->x8 = base[8];
  this->x9 = base[9];
  this->x10 = base[10];
  this->x11 = base[11];
  this->x12 = base[12];
  this->x13 = base[13];
  this->x14 = base[14];
  this->x15 = base[15];
  this->x16 = base[16];
  this->x17 = base[17];
  this->x18 = base[18];
  this->x19 = base[19];
  this->x20 = base[20];
  this->x21 = base[21];
  this->x22 = base[22];
  this->x23 = base[23];
  this->x24 = base[24];
  this->x25 = base[25];
  this->x26 = base[26];
  this->x27 = base[27];
  this->x28 = base[28];
  this->x29 = base[29];
  this->x30 = base[30];
}

void GeneralPurposeRegisterStates::restore_to_sp(uint64_t *base) {
  base[0] = this->x0;
  base[1] = this->x1;
  base[2] = this->x2;
  base[3] = this->x3;
  base[4] = this->x4;
  base[5] = this->x5;
  base[6] = this->x6;
  base[7] = this->x7;
  base[8] = this->x8;
  base[9] = this->x9;
  base[10] = this->x10;
  base[11] = this->x11;
  base[12] = this->x12;
  base[13] = this->x13;
  base[14] = this->x14;
  base[15] = this->x15;
  base[16] = this->x16;
  base[17] = this->x17;
  base[18] = this->x18;
  base[19] = this->x19;
  base[20] = this->x20;
  base[21] = this->x21;
  base[22] = this->x22;
  base[23] = this->x23;
  base[24] = this->x24;
  base[25] = this->x25;
  base[26] = this->x26;
  base[27] = this->x27;
  base[28] = this->x28;
  base[29] = this->x29;
  base[30] = this->x30;
}