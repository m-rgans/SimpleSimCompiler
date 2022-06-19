//***************************************************************************
//
//  simplesim.hpp
//  CSCI 241 Assignment 4
//
//  Created by Alex Peterson (z1912480) and Joshua Sulouff (z1867688)
//
//***************************************************************************
class simplesim
{
  int accumulator;
  int pc; //program counter
  int inst;
  int opcode;
  int operand;
  bool stop;


  void read(int);
  void write(int);

  void store(int);
  void load(int);

  void add(int);
  void subtract(int);
  void mult(int);
  void div(int);
  void branch(int);
  void branchzero(int);
  void branchneg(int);

  void divbyte(int);

  void panic(int);

  int ram[100];
  int rom[100];
public:
  simplesim();
  bool load_program();
  void execute_program();
  void execute_program_verbose();
  void dump();
};
