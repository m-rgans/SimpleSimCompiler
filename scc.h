//***************************************************************************
//
//  scc.hpp
//  CSCI 241 Assignment 9
//
//  Created by Alex Peterson (z1912480) and Joshua Sulouff (z1867688)
//
//***************************************************************************

#ifndef SCC_H
#define SCC_H

#include <string>
#include <vector>
using std::string;

struct sym
{
  string name = "DEFAULT";
  int addr = -1;
  char type = 'n';
  void print();

};

class scc;

class cmdLine
{
public:
  scc* parent = NULL;
  int lnumber = -1;
  string inst = "";
  std::vector<int>  expansion;
  std::vector<char> addrFlags;
  void expand(); //add vram mapping to this step
                  //reference is for looking up syms
  int lookupSym(string, char);
  void print();
private:
  std::vector<int> ASif   (string); //these functions
  void ASdata             (string); //will expand
  std::vector<int> ASlet  (string); //instructions and
  std::vector<int> ASgoto (string); //allocate their syms
  std::vector<int> ASprint(string); //on the sym table.
  std::vector<int> ASinput(string);
};



class scc
{
public:
  static const int RMAX = 100;
  static const int SYMT_L = 100;

  scc();
  void first_pass();
  void second_pass();
  void print_program() const;
  int sym_lookup(const string&, const char&);
  void resDat(string); //reserve data
  void printTables();
  void printASM();
  string symNameByAddr(int);
  void Console_Prog();
private:

  bool linked = false;

  int vram[RMAX];
  char addrFlags[RMAX]; //indicate the kind of addressing
                        //a given instruction needs
                        // s = stack offset, v = variable, l = line number

  cmdLine inst    [RMAX];       //INSTRUCTION TABLE
  sym     sTable  [RMAX];       //SYMBOL TABLE
  int     DatTable[RMAX];       //DATA LIST

  unsigned int nextinst = 0;       //track operations count
  unsigned int nextsym = 0;        //track symbols count
  unsigned int next_dat = 0;
  unsigned int next_var = RMAX - 1;

  void expandItable();

  void allocVars();

  unsigned int lastInstAddr = 0;

  void link();


};

struct reservation
{
  string name = "DEFAULT";
  string content = "0"; //literal value, conditional, etc
  int address; //address where this object is reserved to
  int instructions[32];// the actual instructions this line will be turned into
  char type = 'n'; // n: none e: expression c: condition i: instruction
  bool incomplete = true; //tells compiler that this needs attention on the second pass
};

#endif /* end of include guard: SCC_H */


/*
steps:
Go through and get all the symbols, allocating variable space
expand expressions and conditionals into their full length
convert them into assembly (pseudo), and optimize literal addresses
Assemble resultant program
- All of the operand addresses will be linked during this step, now that we know the addresses of everything
*/
