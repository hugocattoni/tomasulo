// DataMemory.cpp
#include "DataMemory.h"

DataMemory::DataMemory() {
  Qi = nullptr;
  value = 0;
}
int DataMemory::getValue() const { return value; }
int DataMemory::getQi() const { return *Qi; }
int *DataMemory::getQiPointer() const { return Qi; }
void DataMemory::setValue(int newValue) { value = newValue; }
void DataMemory::setQi(int newQi) {
  // novo
  if (Qi == nullptr) {
    Qi = new int;
  }
  *Qi = newQi;
}
void DataMemory::removeQiPointer() { Qi = nullptr; }