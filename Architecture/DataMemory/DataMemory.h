// DataMemory.h
#ifndef DATAMEMORY_H
#define DATAMEMORY_H

class DataMemory {
public:
  DataMemory();
  int getValue() const;
  int getQi() const;
  int *getQiPointer() const;
  void setValue(int newValue);
  void setQi(int newQi);
  void removeQiPointer();

private:
  int *Qi;
  int value;
};

#endif