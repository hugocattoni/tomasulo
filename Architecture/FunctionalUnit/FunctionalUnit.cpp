// FunctionalUnit.cpp
#include "FunctionalUnit.h"

FunctionalUnit::FunctionalUnit() {
  busy = false;
  inst = nullptr;
}

bool FunctionalUnit::getBusy() const { return busy; }
int FunctionalUnit::getInst() const { return *inst; }
int *FunctionalUnit::getInstPointer() const { return inst; }
void FunctionalUnit::setBusy(bool newBusy) { busy = newBusy; }
void FunctionalUnit::setInst(int newInst) {
  // novo
  if (inst == nullptr) {
    inst = new int;
  }
  *inst = newInst;
}