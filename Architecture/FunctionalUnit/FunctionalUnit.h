// FunctionalUnit.h
#ifndef FUNCTIONALUNIT_H
#define FUNCTIONALUNIT_H
#include <iostream>

class FunctionalUnit {
public:
  FunctionalUnit();
  bool getBusy() const;
  int getInst() const;
  int *getInstPointer() const;
  void setBusy(bool newBusy);
  void setInst(int newInst);

private:
  bool busy;
  int *inst;
};

#endif