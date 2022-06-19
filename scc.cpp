//***************************************************************************
//
//  scc.hpp
//  CSCI 241 Assignment 9
//
//  Created by Alex Peterson (z1912480) and Joshua Sulouff (z1867688)
//
//***************************************************************************
#include "scc.h"
#include <iostream>
#include <sstream>

#include "inpost.h"
#include "sml.h"

const int FAILCODE = 1; //change this to 1 before turning in

using std::stringstream;
scc::scc()
{
  for (size_t i = 0; i < RMAX; i++) {
    vram[i] = 7777;
    addrFlags[i] = 'n';
  }

}

/*
* main
* @return success code
****************************/
int main(int argc, char const *argv[]) {
  scc a;
  a.first_pass();
  //a.printTables();

  a.printASM();
  a.Console_Prog();
  return 0;
}

void sym::print()
{
  printf("Symbol %s =====\n", name.c_str());
  printf("Address: %d\n", addr);
  printf("type: %c\n", type);
}

/*
* print machine code to console
* @return none
****************************/
void scc::Console_Prog()
{
  for (size_t i = 0; i < RMAX; i++) {
    if (vram[i] < 0) {
      printf("-");
      vram[i] *= -1;
    }
    else
    {
      printf("+");
    }
    printf("%04d\n", vram[i]);
  }
  printf("-99999\n");

  for (size_t i = 0; i < next_dat; i++) {
    printf("%d\n", DatTable[i]);
  }
}

/*
* print assembly code equivalent of machine code to console
* @return none
****************************/
void scc::printASM()
{
  for (size_t i = 0; i < RMAX; i++) {
    int byte = vram[i];
    int high = byte/100;
    int low = byte%100;
    string command;

    switch (high) {
      case READ: command = "READ"; break;
      case WRITE: command = "WRITE"; break;
      case STORE: command = "STORE"; break;
      case LOAD: command = "LOAD"; break;
      case ADD: command = "ADD"; break;
      case SUBTRACT: command = "SUBTRACT"; break;
      case MULTIPLY: command = "MULTIPLY"; break;
      case DIVIDE: command = "DIVIDE"; break;
      case GOTO: command = "GOTO"; break;
      case BRANCHZERO: command = "BRANCHZERO"; break;
      case BRANCHNEG: command = "BRANCHNEG"; break;
      case HALT: command = "HALT"; break;
      case 0: command = "NOP"; break;
      case 77: command = "NOP"; break;
    }
    //printf("stable %d: %d %d \n", vram[i], high, low );
    string operand = "";
    if (!linked) {
      operand = sTable[low].name;
    }
    else
    {
      operand = symNameByAddr(low);
    }

    if (addrFlags[i] == 'n') {
      operand = "NIL";
    }

    printf("%2lu : %10s %3s",i ,command.c_str(), operand.c_str());

    if (!linked) {
      printf(" (%2d)", sTable[low].addr );
    }
    else
    {
      printf(" (%2d)", low );
    }


    printf(" (Addressing mode: %c) \n", addrFlags[i]);
  }
}

/*
* print debugging info about compiler
* @return none
****************************/
void scc::printTables()
{
  printf("Vars:\nnextinst: %d\nnextsym: %d\nnext_dat: %d\nnext_var %d\n",
                 nextinst,     nextsym,      next_dat,     next_var);
  printf("VRAM\n");
  for (size_t i = 0; i < 10; i++) {
    for (size_t c = 0; c < 10; c++) {
      printf("%4d ", vram[10 * i + c]);
    }
    printf("\n");
  }

  printf("INSTRUCTION TABLE\n");
  for (size_t i = 0; i < nextinst; i++) {
    inst[i].print();
  }

  printf("SYMBOL TABLE\n");
  for (size_t i = 0; i < nextsym; i++) {
    if (sTable[i].addr == -1 && sTable[i].type == 'l') {
      continue;
    }
    printf("Symbol #%lu\n", i);
    sTable[i].print();
  }
}

/*
* Takes the members of the Inst table and puts them in their associated address
* @return none
****************************/
void scc::expandItable()
{
  unsigned int addr = 0;
  unsigned int len;
  //for each inst
  for (size_t i = 0; i < nextinst; i++) {
    cmdLine& thisLine = inst[i];
    sym& thislinesym = sTable[sym_lookup(std::to_string(inst[i].lnumber), 'l')];
    thislinesym.addr = addr;
    //printf("Line %s starts at address %d.\n", thislinesym.name.c_str(), addr );
    len = thisLine.expansion.size();
    //printf("It is %d bytes long.\n", len);

    for (size_t l = 0; l < len; l++) {
      int caddr = addr++;

      if (addr >= RMAX) {
        printf("*** ERROR: ran out Simplesim memory ***\n");
        exit(FAILCODE);
      }

      if (thisLine.expansion.size() > 0) {
        vram     [caddr] = thisLine.expansion[l];
      }

      if (thisLine.addrFlags.size() > 0) {
        addrFlags[caddr] = thisLine.addrFlags[l];
      }
      else
      {
        addrFlags[caddr] = 'n';
      }


    }
  }
  //printf("last instruction: %d\n", addr);
  lastInstAddr = addr;
}

/*
* Correct addresses in the assembly.
* @return none
****************************/
void scc::link()
{
  for (size_t i = 0; i < RMAX; i++) {
    int& byte = vram[i];
    char adMode = addrFlags[i];
    int address = vram[i] % 100;
    int corrected = 0;
    switch (adMode) {
      case 'n':
        break;
      case 's':
        corrected = next_var - address;
        byte -= address;
        byte += corrected;
        if (corrected < (int)lastInstAddr) {
          printf("*** ERROR: insufficient stack space ***\n");
          exit(FAILCODE);
        }
        break;
      case 'v':
        corrected = sTable[address].addr;
        byte -= address;
        byte += corrected;
        break;
      case 'l':
        corrected = sTable[address].addr;
        byte -= address;
        byte += corrected;
        break;
    }
  }

  linked = true;
}

/*
* allocate memory addresses for the variables
* @return none
****************************/
void scc::allocVars()
{
  for (size_t i = 0; i < nextsym; i++) {
    sym& sim = sTable[i];
    if (sim.type == 'v') {
      sim.addr = next_var--;
      if (sim.addr < 0) {
        printf("*** ERROR: ran out Simplesim memory ***\n");
        exit(FAILCODE);
      }
      if (!islower(sim.name[0])) {
        vram[sim.addr] = stoi(sim.name);
      }
      else
      {
        vram[sim.addr] = 0;
      }
    }
  }
}

/*
* Gathers the names for symbols
* @return none
****************************/
void scc::first_pass()
{
  //get the symbol and line tables filled up



  string lin;
  int nextLineAddr = 0;
  while (getline(std::cin, lin)) {
    cmdLine a;
    a.inst = lin;
    a.parent = this;
    a.expand();
    nextLineAddr += a.expansion.size();

    //add instruction to table

    inst[nextinst] = a;
    nextinst++;

  }
  expandItable();
  allocVars();
  link();
}

/*
* Reserves a slot for data
* @return none
****************************/
void scc::resDat(string a)
{
  if (next_dat >= RMAX) {
    std::cout << "*** ERROR: too many data lines ***\n";
    exit(FAILCODE);
    return;
  }

  DatTable[next_dat] = stoi(a);
  next_dat++;
  return;
}

/*
* Looks up a symbol and creates it, or returns its index in the symbol table
* @return int - index in sym table
****************************/
int scc::sym_lookup(const string& name, const char& type )
{
//  char lookupChar = name[0];
  //printf("Looking up symbol %s\n", name.c_str());
  for (size_t i = 0; i < nextsym; i++) {
      if ((sTable[i].name == name) && (sTable[i].type == type)) {
        //printf("Found identical symbol at %lu\n", i);
        return i;
      }
  }

  if (nextsym >= RMAX) {
    printf("*** ERROR: ran out Simplesim memory ***\n");
    exit(FAILCODE);
    return -1;
  }

  sym newsim;
  newsim.name = name;
  newsim.type = type;
  sTable[nextsym] = newsim;
  return nextsym++;
}

/*
* print debugging info about compiler
* @param - name of the symbol
* @param - type of symbol (line, variable)
* @return name of the variable. Used for assembler output.
****************************/
string scc::symNameByAddr(int addr)
{
  if ((addr >= (int)lastInstAddr) && (addr <= (int)next_var)) {
    return "STK";
  }
  for (size_t i = 0; i < nextsym; i++) {
    sym& thisSym = sTable[i];
    if( thisSym.addr == addr)
    {return thisSym.name;}
  }
  return "NOT FOUND.";
}

std::vector<int> ASif   (string);
void ASdata             (string);
std::vector<int> ASlet  (string);
std::vector<int> ASgoto (string);
std::vector<int> ASprint(string);
std::vector<int> ASinput(string);

bool issym(string a);

/*
* expands the line from its basic instructions, into equivalent assembler commands
* @param - expression to conver
* @return none
****************************/
void cmdLine::expand()
{
  string key;

  stringstream take(inst);
  take >> lnumber; take >> key;

  switch (key[0]) {
    case 'i':
      if (key == "if") {expansion = ASif(inst);}
      else {expansion = ASinput(inst);}
      break;
    case 'd':
      ASdata(inst);
      break;
    case 'l':
      expansion = ASlet(inst);
      break;
    case 'g':
      expansion = ASgoto(inst);
      break;
    case 'p':
      expansion = ASprint(inst);
      break;
    case 'e':
      expansion.push_back(HALT * 100);
      break;
    case 'r':
      break;
    default:
      printf("Couldn't identify key!\n");
      break;
  }
  lookupSym(std::to_string(lnumber), 'l');
  return;
}

/*
* expands an if statement into sml
* @param - expression to convert
* @return vector of sml commands
****************************/
std::vector<int> cmdLine::ASif(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string b;
  s >> b; // line no.
  s >> b; // if
  int lop , rop, line;
  string op;
  s >> b; // a
  lop = lookupSym(b, 'v');
  s >> op; // op
  s >> b; //b
  rop = lookupSym(b, 'v');
  s >> b; //goto
  s >> b; //line
  line = lookupSym(b, 'l');

  //printf("Lop: %d Rop: %d Op: %s\n",lop, rop, op.c_str() );

  switch (op[0]) {
    case '=':
      v.push_back( (LOAD * 100) + lop );
      addrFlags.push_back('v');
      v.push_back( (SUBTRACT * 100) + rop );
      addrFlags.push_back('v');
      v.push_back( (BRANCHZERO * 100) + line );
      addrFlags.push_back('l');
      break;
    case '>':
      //printf("Greater than statement.\n");
      v.push_back( (LOAD * 100) + rop );
      addrFlags.push_back('v');
      v.push_back( (SUBTRACT * 100) + lop );
      addrFlags.push_back('v');
      v.push_back( (BRANCHNEG * 100) + line );
      addrFlags.push_back('l');
      if (op.length() == 2)
      {
        v.push_back( (BRANCHZERO * 100) + line );
        addrFlags.push_back('l');
      }
      break;
    case '<':
      //printf("less than statement.\n");
      v.push_back( (LOAD * 100) + lop );
      addrFlags.push_back('v');
      v.push_back( (SUBTRACT * 100) + rop );
      addrFlags.push_back('v');
      v.push_back( (BRANCHNEG * 100) + line );
      addrFlags.push_back('l');
      if (op.length() == 2) {
        v.push_back( (BRANCHZERO * 100) + line );
        addrFlags.push_back('l');
      }
      break;
  }


  return v;
  //filler
}

/*
* expands an input statement into sml
* @param - expression to convert
* @return vector of sml commands
****************************/
std::vector<int> cmdLine::ASinput(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string b; //303 input x
  s >> b; // line no.
  s >> b; // input keyword
  s >> b; // sym
  int sym = lookupSym(b, 'v');
  v.push_back((READ * 100) + sym);
  addrFlags.push_back('v');
  return v;
}

/*
* expands a data statement into sml
* @param - expression to convert
* @return vector of sml commands
****************************/
void cmdLine::ASdata(string inst)
{
  string b;
  stringstream s(inst);
  s >> b; // line no.
  s >> b; // data keyword
  s >> b; // sym
  parent->resDat(b);
  //printf("Appending number to end of file: %s\n", b.c_str());
}

/*
* expands a let statement into sml
* @param - expression to convert
* @return vector of sml commands
****************************/
std::vector<int> cmdLine::ASlet(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string target;
  s >> target; //line no
  s >> target; //let
  s >> target; //sym
  int targetsym = lookupSym(target, 'v');
  //printf("Got target: %s\n", target.c_str());

  string exp;
  string temp;
  s>>temp;
  while (s >> temp) {exp += temp;}
  //printf("Got exp: %s\n", exp.c_str());

  string pfix = convert(exp);
  //printf("Converted to %s\n", pfix.c_str());

  stringstream p(pfix);

  string token;

  int sp = 1; //SP IS ALWAYS NEXT OPEN STACK POSITION

  //printf("First token: %s\n", token.c_str());
  while (p >> token) {
    //printf("sp: %d\n", sp);
    if (issym(token)) {
      v.push_back( (LOAD * 100) + lookupSym(token, 'v') );
      addrFlags.push_back('v');
      v.push_back( (STORE * 100) + sp ); sp++;
      addrFlags.push_back('s');
    }
    else
    {
      if (sp <= 0) {
        printf("Stack pointer is off!\n");
      }
      if (token == "+") {
        v.push_back( (LOAD * 100) + (sp-1)); sp--; //pop into acc
        addrFlags.push_back('s');
        v.push_back( (ADD * 100) + (sp-1) ); sp--; //add to accumator and pop
        addrFlags.push_back('s');
        v.push_back( (STORE * 100) + sp ); sp++;   //put back on stack
        addrFlags.push_back('s');
      }
      else if (token == "-"){
        v.push_back( (LOAD * 100) + (sp-1)); sp--; //pop first operand off stack
        addrFlags.push_back('s');
        v.push_back( (STORE * 100) + (0));         //put operand in special position
        addrFlags.push_back('s');
        v.push_back( (LOAD * 100) + (sp-1)); sp--; //pop first operand off stack
        addrFlags.push_back('s');
        v.push_back( (SUBTRACT * 100) + (0) );     //subtract special position
        addrFlags.push_back('s');
        v.push_back( (STORE * 100) + sp ); sp++;   //push result
        addrFlags.push_back('s');
      }
      else if (token == "/"){
        v.push_back( (LOAD * 100) + (sp-1)); sp--; //pop first operand off stack
        addrFlags.push_back('s');
        v.push_back( (STORE * 100) + (0));         //put operand in special position
        addrFlags.push_back('s');
        v.push_back( (LOAD * 100) + (sp-1)); sp--; //pop first operand off stack
        addrFlags.push_back('s');
        v.push_back( (DIVIDE * 100) + (0) );     //divide special position
        addrFlags.push_back('s');
        v.push_back( (STORE * 100) + sp ); sp++;   //push result
        addrFlags.push_back('s');
      }
      else if (token == "*"){
        v.push_back( (LOAD * 100) + (sp-1)); sp--;
        addrFlags.push_back('s');
        v.push_back( (MULTIPLY * 100) + (sp-1) ); sp--;
        addrFlags.push_back('s');
        v.push_back( (STORE * 100) + sp ); sp++;
        addrFlags.push_back('s');
      }
    }
  }
  //printf("final sp %d\n", sp);
  v.push_back( (LOAD * 100) + (sp-1)); sp--; //pop result off stack
  addrFlags.push_back('s');
  v.push_back( (STORE * 100) + targetsym );
  addrFlags.push_back('v');

  return v;
  //filler
}
#include <cctype>

/*
* Used by ASlet to determine whether something is a value or an operator
* @return bool whether the argument is a symbol
****************************/
bool issym(string a)
{
  int b = a.length();
  for (int i = 0; i < b; i++) {
    if (!(isdigit(a[i]) || islower(a[i]))) {return false;}
  }
  return true;
}

/*
* expands a goto statement into sml
* @param - expression to convert
* @return vector of sml commands
****************************/
std::vector<int> cmdLine::ASgoto(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string b;
  s >> b; // line no.
  s >> b; // goto keyword
  s >> b; // sym
  int sym = lookupSym(b, 'l');
  v.push_back((GOTO * 100) + sym);
  addrFlags.push_back('l');
  return v;
}

/*
* expands a print statement into sml
* @param - expression to convert
* @return vector of sml commands
****************************/
std::vector<int> cmdLine::ASprint(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string b;
  s >> b; //line no.
  s >> b; //print keyword
  s >> b; //symbol
  int sym = lookupSym(b, 'v');
  addrFlags.push_back('v');
  v.push_back((WRITE * 100) + sym);
  return v;

}


/*
* Shows information about a line for debugging info
* @return none
****************************/
void cmdLine::print()
{
  printf("Literal instruction:\n%s\n", inst.c_str());

  printf("Expands to:\n");
  int len = expansion.size();
  for (int i = 0; i < len; i++) {
    printf("%d\n", expansion[i]);
  }
}

/*
* Wrapper for cmdLine to look up symbols in the parent scc object
* @return int symbol index
****************************/
int cmdLine::lookupSym(string a, char type)
{
  if (parent == NULL) {
    printf("Something is very wrong.\n");
    return 0;
  }
  return (parent->sym_lookup(a,type));
}
