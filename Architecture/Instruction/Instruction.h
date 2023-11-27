// Instruction.h
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>

class Instruction {
public:
  Instruction(std::string name, int *dest, int *r1, int *r2, int *imm);
  std::string toString() const;
  std::string getName() const;
  int getDest() const;
  int getR1() const;
  int getR2() const;
  int getImm() const;

private:
  std::string name;
  int *dest;
  int *r1;
  int *r2;
  int *imm;
};

#endif