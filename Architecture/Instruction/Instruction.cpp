// Instruction.cpp
#include "Instruction.h"

Instruction::Instruction(std::string name, int *dest, int *r1, int *r2,
                         int *imm)
    : name(name), dest(dest), r1(r1), r2(r2), imm(imm) {}

std::string Instruction::toString() const {
  std::string result = name + " ";
  if (dest != nullptr) {
    result += std::to_string(*dest) + " ";
  } else {
    result += "NULL ";
  }
  if (r1 != nullptr) {
    result += std::to_string(*r1) + " ";
  } else {
    result += "NULL ";
  }
  if (r2 != nullptr) {
    result += std::to_string(*r2) + " ";
  } else {
    result += "NULL ";
  }
  if (imm != nullptr) {
    result += std::to_string(*imm);
  } else {
    result += "NULL";
  }
  return result;
}

std::string Instruction::getName() const { return name; }

int Instruction::getDest() const { return *dest; }
int Instruction::getR1() const { return *r1; }
int Instruction::getR2() const { return *r2; }
int Instruction::getImm() const { return *imm; }
