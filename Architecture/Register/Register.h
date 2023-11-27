#include <string>

// Register.h
#ifndef REGISTER_H
#define REGISTER_H

class Register {
public:
  Register();
  int *getQiPointer() const;
  int getQi() const;
  int getValue() const;
  void setQi(int newQi);
  void setValue(int newValue);
  void removeQiPointer();
  std::string toString() const;

private:
  int *Qi;
  int value;
};

#endif