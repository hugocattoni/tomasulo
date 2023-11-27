// ReservationStation.h
#ifndef RESERVATIONSTATION_H
#define RESERVATIONSTATION_H

#include <string>

class ReservationStation {
public:
  ReservationStation();
  bool getBusy() const;
  int getExec() const;
  std::string getOp() const;
  std::string *getOpPointer() const;
  int getVk() const;
  int *getVkPointer() const;
  int getQj() const;
  int *getQjPointer() const;
  int getQk() const;
  int *getQkPointer() const;
  int getVj() const;
  int *getVjPointer() const;
  int getA() const;
  void setBusy(bool newBusy);
  void setExec(int newExec);
  void setOp(const std::string &newOp);
  void setVk(int newVk);
  void setQj(int newQj);
  void removeQjPointer();
  void setQk(int newQk);
  void removeQkPointer();
  void setVj(int newVj);
  void setA(int newA);
  std::string toString() const;

private:
  bool busy;
  int *exec;
  std::string *op;
  int *Vj;
  int *Vk;
  int *Qj;
  int *Qk;
  int *A;
};

#endif