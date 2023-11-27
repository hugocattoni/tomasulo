// Register.cpp
#include "Register.h"
#include <sstream>

Register::Register() {
  Qi = nullptr;
  value = 0;
}

int *Register::getQiPointer() const { return Qi; }
int Register::getQi() const { return *Qi; }
int Register::getValue() const { return value; }
void Register::setQi(int newQi) {
  // novo
  if (Qi == nullptr) {
    Qi = new int;
  }
  *Qi = newQi;
}
void Register::setValue(int newValue) { value = newValue; }
void Register::removeQiPointer() { Qi = nullptr; }
std::string Register::toString() const {
  std::ostringstream oss;
  oss << (Qi != nullptr ? std::to_string(*Qi) : "NONE") << " | ";
  oss << (std::to_string(value));

  return oss.str();
}