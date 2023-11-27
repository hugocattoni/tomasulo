// ReservationStation.cpp
#include "ReservationStation.h"
#include <iostream>
#include <sstream>

ReservationStation::ReservationStation() {
  busy = false;
  exec = nullptr;
  op = nullptr;
  Vj = nullptr;
  Vk = nullptr;
  Qj = nullptr;
  Qk = nullptr;
  A = nullptr;
}

bool ReservationStation::getBusy() const { return busy; }
int ReservationStation::getExec() const {
  // novo
  if (exec != nullptr) {
    // std::cout << "NAO ZERITO" << std::endl;
    return *exec;
  } else {
    // std::cout << "ZERITO" << std::endl;
    return 0;
  }
}
std::string ReservationStation::getOp() const { return *op; }
std::string *ReservationStation::getOpPointer() const { return op; }
int ReservationStation::getVk() const { return *Vk; }
int *ReservationStation::getVkPointer() const { return Vk; }
int ReservationStation::getQj() const { return *Qj; }
int *ReservationStation::getQjPointer() const { return Qj; }
int ReservationStation::getQk() const { return *Qk; }
int *ReservationStation::getQkPointer() const { return Qk; }
int ReservationStation::getVj() const {
  // novo
  if (Vj != nullptr) {
    return *Vj;
  } else {

    return 0;
  }
}
int *ReservationStation::getVjPointer() const {
  // novo
  if (Vj != nullptr) {
    return Vj;
  } else {

    return nullptr;
  }
}
int ReservationStation::getA() const { return *A; }
void ReservationStation::setBusy(bool newBusy) { busy = newBusy; }
void ReservationStation::setExec(int newExec) {
  // novo
  if (exec == nullptr) {
    exec = new int;
  }
  *exec = newExec;
}
void ReservationStation::setOp(const std::string &newOp) {
  // novo
  if (op == nullptr) {
    op = new std::string;
  }
  *op = newOp;
}
void ReservationStation::setVk(int newVk) {
  // novo
  if (Vk == nullptr) {
    Vk = new int;
  }
  *Vk = newVk;
}
void ReservationStation::setQj(int newQj) {
  // novo
  if (Qj == nullptr) {
    Qj = new int;
  }
  *Qj = newQj;
}
void ReservationStation::removeQjPointer() { Qj = nullptr; }
void ReservationStation::setQk(int newQk) {
  // novo
  if (Qk == nullptr) {
    Qk = new int;
  }
  *Qk = newQk;
}
void ReservationStation::removeQkPointer() { Qk = nullptr; }
void ReservationStation::setVj(int newVj) {
  // novo
  if (Vj == nullptr) {
    Vj = new int;
  }
  *Vj = newVj;
}
void ReservationStation::setA(int newA) { // novo
  if (A == nullptr) {
    A = new int;
  }
  *A = newA;
}

std::string ReservationStation::toString() const {
  std::ostringstream oss;
  oss << (busy ? "true" : "false") << " | ";
  oss << (exec != nullptr ? std::to_string(*exec) : "NONE") << " | ";
  oss << (op != nullptr ? *op : "NONE") << " | ";
  oss << (Vj != nullptr ? std::to_string(*Vj) : "NONE") << " | ";
  oss << (Vk != nullptr ? std::to_string(*Vk) : "NONE") << " | ";
  oss << (Qj != nullptr ? std::to_string(*Qj) : "NONE") << " | ";
  oss << (Qk != nullptr ? std::to_string(*Qk) : "NONE") << " | ";
  oss << (A != nullptr ? std::to_string(*A) : "NONE");

  return oss.str();
}
