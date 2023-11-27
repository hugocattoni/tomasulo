#include "Architecture/DataMemory/DataMemory.h"
#include "Architecture/FunctionalUnit/FunctionalUnit.h"
#include "Architecture/Instruction/Instruction.h"
#include "Architecture/Register/Register.h"
#include "Architecture/ReservationStation/ReservationStation.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

int clockCycle = 0;
int instructions = 0;
int rsAddI = 0;
int rsMulI = 16;
int rsLdI = 32;
std::vector<FunctionalUnit> addUnit(3);
std::vector<FunctionalUnit> mulUnit(3);
std::vector<FunctionalUnit> ldUnit(3);
std::vector<DataMemory> memory(512);
std::vector<Register> reg(32);
std::vector<ReservationStation> reservationStation(48);
std::ofstream output("output.txt");

std::vector<Instruction> createInstructionQueue(std::ifstream &arq) {
  std::vector<Instruction> queue;
  std::string line;

  while (std::getline(arq, line)) {
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> tokens;

    while (ss >> word) {
      word.erase(std::remove(word.begin(), word.end(), ','), word.end());
      tokens.push_back(word);
    }

    std::string name = tokens[0];
    if (tokens.size() > 3) {
      int *dest = new int(std::stoi(tokens[1].substr(1)));
      int *r1 = new int(std::stoi(tokens[2].substr(1)));
      int *r2 = new int(std::stoi(tokens[3].substr(1)));

      Instruction inst(name, dest, r1, r2, nullptr);
      queue.push_back(inst);
    } else if (tokens.size() == 3) {
      int *dest = new int(std::stoi(tokens[1].substr(1)));
      std::string imm_rs = tokens[2];
      size_t pos = imm_rs.find("(");
      int *immPtr = new int(std::stoi(imm_rs.substr(0, pos)));
      size_t reg_pos = imm_rs.find("R");
      int *rs = new int(std::stoi(imm_rs.substr(reg_pos + 1)));

      Instruction inst(name, dest, rs, nullptr, immPtr);
      queue.push_back(inst);
    }
  }
  return queue;
}

void issue(Instruction inst) {
  std::string op = inst.getName();
  int r = 0;

  std::vector<std::string> arithOps = {"ADD", "SUB", "MUL", "DIV"};
  std::vector<std::string> memOps = {"LW", "SW"};

  if (std::find(arithOps.begin(), arithOps.end(), op) != arithOps.end()) {
    if (op == "MUL" || op == "DIV") {
      r = 16;
      while (reservationStation[r].getBusy() && r < 32) {
        ++r;
      }
    } else {
      while (reservationStation[r].getBusy() && r < 16) {
        ++r;
      }
    }

    int rd = inst.getDest();
    int rs = inst.getR1();
    int rt = inst.getR2();

    // verificar se os registradores de origem (rs e rt) da instrucao estao sendo usados por outra instrucao
    // armazenar o identificador da estação de reserva se estiver ocupado
    // armazenar o valor do registrador
    if (reg[rs].getQiPointer() != nullptr) {
      reservationStation[r].setQj(reg[rs].getQi());
    } else {
      reservationStation[r].setVj(reg[rs].getValue());
    }

    if (reg[rt].getQiPointer() != nullptr) {
      reservationStation[r].setQk(reg[rt].getQi());
    } else {
      reservationStation[r].setVk(reg[rt].getValue());
    }
    
    // marcar estação de reserva como ocupada
    // o registrador destino (se existir) é marcado com o identificador da estação de reserva, ou seja, depende do resultado de r
    reservationStation[r].setBusy(true);
    reg[rd].setQi(r);

  } 
  else if (std::find(memOps.begin(), memOps.end(), op) != memOps.end()) {
    r = 32;
    while (reservationStation[r].getBusy() && r < 48) {
      ++r;
    }

    int rd = inst.getDest();
    int imm = inst.getImm();
    int rs = inst.getR1();

    if (op == "LW") {
      reservationStation[r].setVj(memory[rs].getValue());

      // marcar estação de reserva como ocupada
      // definir endereço a ser utilizado na operação
      reservationStation[r].setA(imm);
      reservationStation[r].setBusy(true);
      reg[rd].setQi(r);

    } else if (op == "SW") {
      // verificar se os registradores de origem (rs e rt) da instrucao estao sendo usados por outra instrucao
      // armazenar o identificador da estação de reserva se estiver ocupado
      // armazenar o valor do registrador
      if (reg[rs].getQiPointer() != nullptr) {
        reservationStation[r].setQj(reg[rs].getQi());
      } else {
        reservationStation[r].setVj(reg[rs].getValue());
      }

      // marcar estação de reserva como ocupada
      // definir endereço a ser utilizado na operação
      reservationStation[r].setA(imm);
      reservationStation[r].setBusy(true);

      // o registrador destino (se existir) é marcado com o identificador da estação de reserva, ou seja, depende do resultado de r
      memory[rd].setQi(r);
    }
  } 
  else {
    throw std::runtime_error("Instrução não reconhecida");
  }

  // definir contador de execução, operação na estação de reserva e incrementar contador de instruções
  reservationStation[r].setExec(-1);
  reservationStation[r].setOp(op);
  instructions++;
}

int simulateExec(int inst) {
  if (reservationStation[inst].getOp() == "ADD")
    return reservationStation[inst].getVj() + reservationStation[inst].getVk();
  if (reservationStation[inst].getOp() == "SUB")
    return reservationStation[inst].getVj() - reservationStation[inst].getVk();
  if (reservationStation[inst].getOp() == "MUL")
    return reservationStation[inst].getVj() * reservationStation[inst].getVk();
  if (reservationStation[inst].getOp() == "DIV")
    return reservationStation[inst].getVj() / reservationStation[inst].getVk();
  if (reservationStation[inst].getOp() == "LW")
    return reservationStation[inst].getVj() + reservationStation[inst].getA();
  if (reservationStation[inst].getOp() == "SW")
    return reservationStation[inst].getVj() + reservationStation[inst].getA();

  std::cout << "OPERAÇÃO INVÁLIDA" << std::endl;
  return -1;
}

void write() {
  for (auto &unit : addUnit) {
    // verificar se a instrução correspondente na estação de reserva está pronta para ser escrita
    if (unit.getBusy() && reservationStation[unit.getInst()].getExec() == 0) {
      int v = simulateExec(unit.getInst());

      for (auto &regist : reg) {
        // rever
        if (regist.getQiPointer() != nullptr &&
            regist.getQi() == unit.getInst()) {
          regist.removeQiPointer();
          regist.setValue(v);
        }
      }

      for (auto &rs : reservationStation) {
        // rever
        if (rs.getQjPointer() != nullptr && rs.getQj() == unit.getInst()) {
          rs.setVj(v);
          rs.removeQjPointer();
        }
        // rever
        if (rs.getQkPointer() != nullptr && rs.getQk() == unit.getInst()) {
          rs.setVk(v);
          rs.removeQkPointer();
        }
      }
      instructions--;
      reservationStation[unit.getInst()] = ReservationStation();
      unit.setBusy(false);
    }
  }

  for (auto &unit : mulUnit) {
    if (unit.getBusy() && reservationStation[unit.getInst()].getExec() == 0) {
      int v = simulateExec(unit.getInst());
      for (auto &regist : reg) {

        if (regist.getQiPointer() != nullptr &&
            regist.getQi() == unit.getInst()) {
          regist.removeQiPointer();
          regist.setValue(v);
        }
      }
      for (auto &rs : reservationStation) {
        if (rs.getQjPointer() != nullptr && rs.getQj() == unit.getInst()) {
          rs.setVj(v);
          rs.removeQjPointer();
        }
        if (rs.getQkPointer() != nullptr && rs.getQk() == unit.getInst()) {
          rs.setVk(v);
          rs.removeQkPointer();
        }
      }
      instructions--;
      reservationStation[unit.getInst()] = ReservationStation();
      unit.setBusy(false);
    }
  }

  for (auto &unit : ldUnit) {
    if (unit.getBusy() && reservationStation[unit.getInst()].getExec() == 0) {
      int v = simulateExec(unit.getInst());

      // ver esse dps
      // if (reservationStation[unit.getInst()].getOpPointer() &&
      //     reservationStation[unit.getInst()].getOp() == "SW") {
      if (reservationStation[unit.getInst()].getOp() == "SW") {
        for (auto &mem : memory) {
          if (mem.getQiPointer() != nullptr && mem.getQi() == unit.getInst()) {
            mem.removeQiPointer();
            mem.setValue(v);
          }
        }
      }

      else {
        for (auto &regist : reg) {
          if (regist.getQiPointer() != nullptr &&
              regist.getQi() == unit.getInst()) {
            regist.removeQiPointer();
            regist.setValue(v);
          }
        }
      }
      for (auto &rs : reservationStation) {
        if (rs.getQjPointer() != nullptr && rs.getQj() == unit.getInst()) {
          rs.setVj(v);
          rs.removeQjPointer();
        }
        if (rs.getQkPointer() != nullptr && rs.getQk() == unit.getInst()) {
          rs.setVk(v);
          rs.removeQkPointer();
        }
      }
      instructions--;
      reservationStation[unit.getInst()] = ReservationStation();
      unit.setBusy(false);
    }
  }
}

void executeInstructions() {
  for (auto &unit : addUnit) {
    if (unit.getBusy()) {
      reservationStation[unit.getInst()].setExec(
          reservationStation[unit.getInst()].getExec() - 1);
    }
  }

  for (auto &unit : mulUnit) {
    if (unit.getBusy()) {
      reservationStation[unit.getInst()].setExec(
          reservationStation[unit.getInst()].getExec() - 1);
    }
  }

  for (auto &unit : ldUnit) {
    if (unit.getBusy()) {
      reservationStation[unit.getInst()].setExec(
          reservationStation[unit.getInst()].getExec() - 1);
    }
  }

  for (int i = 0; i < 16; ++i) {
    if (reservationStation[rsAddI].getBusy() &&
        reservationStation[rsAddI].getExec() == -1 &&
        reservationStation[rsAddI].getVjPointer() != nullptr &&
        reservationStation[rsAddI].getVkPointer() != nullptr) {
      if (!addUnit[0].getBusy()) {
        addUnit[0].setInst(rsAddI);
        addUnit[0].setBusy(true);
      } else if (!addUnit[1].getBusy()) {
        addUnit[1].setInst(rsAddI);
        addUnit[1].setBusy(true);
      } else if (!addUnit[2].getBusy()) {
        addUnit[2].setInst(rsAddI);
        addUnit[2].setBusy(true);
      } else {
        break;
      }
      reservationStation[rsAddI].setExec(5);
    }
    rsAddI = (rsAddI + 1) % 16;

    if (reservationStation[rsMulI].getBusy() &&
        reservationStation[rsMulI].getExec() == -1 &&
        reservationStation[rsMulI].getVjPointer() != nullptr &&
        reservationStation[rsMulI].getVkPointer() != nullptr) {
      if (!mulUnit[0].getBusy()) {
        mulUnit[0].setInst(rsMulI);
        mulUnit[0].setBusy(true);

      } else if (!mulUnit[1].getBusy()) {
        mulUnit[1].setInst(rsMulI);
        mulUnit[1].setBusy(true);

      } else if (!mulUnit[2].getBusy()) {
        mulUnit[2].setInst(rsMulI);
        mulUnit[2].setBusy(true);
      } else {
        break;
      }
      if (reservationStation[rsMulI].getOp() == "MUL") {
        reservationStation[rsMulI].setExec(15);
      } else {
        reservationStation[rsMulI].setExec(25);
      }
    }
    rsMulI = ((rsMulI + 1) % 16) + 16;

    if (reservationStation[rsLdI].getBusy() &&
        reservationStation[rsLdI].getExec() == -1 &&
        reservationStation[rsLdI].getVjPointer() != nullptr) {

      if (!ldUnit[0].getBusy()) {
        ldUnit[0].setInst(rsLdI);
        ldUnit[0].setBusy(true);
      } else if (!ldUnit[1].getBusy()) {
        ldUnit[1].setInst(rsLdI);
        ldUnit[1].setBusy(true);
      } else if (!ldUnit[2].getBusy()) {
        ldUnit[2].setInst(rsLdI);
        ldUnit[2].setBusy(true);
      } else {
        break;
      }
      reservationStation[rsLdI].setExec(5);
    }
    rsLdI = ((rsLdI + 1) % 16) + 32;
  }
}

void writeOutput() {
  if (output.is_open()) {
    output << "================================================================"
              "================\n";
    output << "Clock: " << clockCycle << "\n";
    output << " Name  | BUSY  | Clock |   OP   |   Vj  |   Vk  |   Qj  |   Qk  "
              "|   A   |\n";
    for (int i = 0; i < 16; ++i) {
      if (reservationStation[i].getBusy()) {
        output << "ADD " << i << " | " << reservationStation[i].toString()
               << "\n";
      }
    }
    for (int i = 16; i < 32; ++i) {
      if (reservationStation[i].getBusy()) {
        output << "MUL " << i << " | " << reservationStation[i].toString()
               << "\n";
      }
    }
    for (int i = 32; i < 48; ++i) {
      if (reservationStation[i].getBusy()) {
        output << "LOA " << i << " | " << reservationStation[i].toString()
               << "\n";
      }
    }

    output << "\nRegistradores:\n";
    output << "Reg | Qi | Value\n";

    for (int i = 0; i < reg.size(); ++i) {
      output << i << " | " << reg[i].toString() << "\n";
    }
  }
}

int main() {
  std::ifstream input("in.txt");
  if (!input.is_open()) {
    std::cout << "Erro ao abrir o arquivo.\n";
    return 1;
  }

  std::vector<Instruction> instr_queue = createInstructionQueue(input);

  // imprimir instrucoes
  for (const auto &instruction : instr_queue) {
    std::cout << instruction.toString() << std::endl;
  }

  issue(instr_queue.front());
  instr_queue.erase(instr_queue.begin());

  while (instructions > 0) {
    // std::cout << "\nCLOCK: " << clockCycle << std::endl;
    write();
    executeInstructions();
    writeOutput();
    clockCycle++;
    if (instr_queue.size() > 0) {
      issue(instr_queue.front());
      instr_queue.erase(instr_queue.begin());
    }
  }

  instr_queue.clear();
  input.close();
  output.close();
  return 0;
}
