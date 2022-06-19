/***************************************************************
CSCI 240         Program 0     Fall 2020

Programmer: Joshua Sulouff

Section: 2

Date Due:

Purpose:
***************************************************************/

#include<string>
#include<vector>
#include<sstream>
#include"sml.h"
#include"inpost.h"
using std::string;
using std::stringstream;

/*
if    - branch depending on operator
data  - append to back of file, after 99999.
let   - evaluate the exp and assign the key to it
rem   - nothing
goto  - jmp instruction
print - print instruction
input - read from post-HALT area
*/

//gather symbols
//expand lines
//allocate variables
//designate stack
//fill in addresses

class cmdLine
{
public:
  int lnumber = -1;
  string inst = "";
  std::vector<int> expansion;
  //std::vector<string> dbexpansion;
  void expand();
  void print();
};

std::vector<int> ASif   (string);
void ASdata             (string);
std::vector<int> ASlet  (string);
std::vector<int> ASgoto (string);
std::vector<int> ASprint(string);
std::vector<int> ASinput(string);
int lookupSym(string a);
bool issym(string a);

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
      expansion.push_back(HALT);
      break;
    case 'r';
      break;
    default:
      printf("Couldn't identify key!\n");
      break;
  }
  return;
}

std::vector<int> ASif(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string b;
  s >> b; // line no.
  s >> b; // if
  int lop , rop, line;
  string op;
  s >> b; // a
  lop = lookupSym(b);
  s >> op; // op
  s >> b; //b
  rop = lookupSym(b);
  s >> b; //goto
  s >> b; //line
  line = lookupSym(b);

  printf("Lop: %d Rop: %d Op: %s\n",lop, rop, op.c_str() );

  switch (op[0]) {
    case '=':
      v.push_back( (LOAD * 100) + lop );
      v.push_back( (SUBTRACT * 100) + rop );
      v.push_back( (BRANCHZERO * 100) + line );
      break;
    case '>':
      printf("Greater than statement.\n");
      v.push_back( (LOAD * 100) + lop );
      v.push_back( (SUBTRACT * 100) + rop );
      if (op.length() == 2) { v.push_back( (BRANCHZERO * 100) + line ); }
      v.push_back( (BRANCHNEG * 100) + line );
      break;
    case '<':
      printf("less than statement.\n");
      v.push_back( (LOAD * 100) + rop );
      v.push_back( (SUBTRACT * 100) + lop );
      if (op.length() == 2) { v.push_back( (BRANCHZERO * 100) + line ); }
      v.push_back( (BRANCHNEG * 100) + line );
      break;
  }


  return v;
  //filler
}

std::vector<int> ASinput(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string b; //303 input x
  s >> b; // line no.
  s >> b; // input keyword
  s >> b; // sym
  int sym = lookupSym(b);
  v.push_back((READ * 100) + sym);
  return v;
}

void ASdata(string inst)
{
  string b;
  stringstream s(inst);
  s >> b; // line no.
  s >> b; // data keyword
  s >> b; // sym
  printf("Appending number to end of file: %s\n", b.c_str());
}

//broken
std::vector<int> ASlet(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string target;
  s >> target; //line no
  s >> target; //let
  s >> target; //sym
  int targetsym = lookupSym(target);
  printf("Got target: %s\n", target.c_str());

  string exp;
  string temp;
  s>>temp;
  while (s >> temp) {exp += temp;}
  printf("Got exp: %s\n", exp.c_str());

  string pfix = convert(exp);
  printf("Converted to %s\n", pfix.c_str());

  stringstream p(pfix);

  string token;

  int sp = 0;

  p >> token;
  v.push_back( (LOAD * 100) + lookupSym(token) );

  printf("First token: %s\n", token.c_str());
  while (p >> token) {
    printf("sp: %d\n", sp);
    if (issym(token)) {
      v.push_back( (STORE * 100) + sp ); sp++;
      v.push_back( (LOAD * 100) + lookupSym(token) );
    }
    else
    {
      if (token == "+") {
        v.push_back( (ADD * 100) + (sp-1) ); sp--;
      }
      else if (token == "-"){
        v.push_back( (SUBTRACT * 100) + (sp-1) ); sp--;
      }
      else if (token == "/"){
        v.push_back( (DIVIDE * 100) + (sp-1) ); sp--;
      }
      else if (token == "*"){
        v.push_back( (MULTIPLY * 100) + (sp-1) ); sp--;
      }
    }
  }
  printf("final sp %d\n", sp);

  return v;
  //filler
}
#include <cctype>

bool issym(string a)
{
  int b = a.length();
  for (size_t i = 0; i < b; i++) {
    if (!(isdigit(a[i]) || islower(a[i]))) {return false;}
  }
  return true;
}

std::vector<int> ASgoto(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string b;
  s >> b; // line no.
  s >> b; // goto keyword
  s >> b; // sym
  int sym = lookupSym(b);
  v.push_back((GOTO * 100) + sym);
  return v;
}

std::vector<int> ASprint(string inst)
{
  std::vector<int> v;
  stringstream s(inst);
  string b;
  s >> b; //line no.
  s >> b; //print keyword
  s >> b; //symbol
  int sym = lookupSym(b);
  v.push_back((WRITE * 100) + sym);
  return v;

}



void cmdLine::print()
{
  printf("Literal instruction:\n%s\n", inst.c_str());

  printf("Expands to:\n");
  int len = expansion.size();
  for (size_t i = 0; i < len; i++) {
    printf("%d\n", expansion[i]);
  }
}

int lookupSym(string a)
{
  if (a == "x") {return 99;}
  else if (a == "d")   {return 98;}
  else if (a == "z")   {return 97;}
  else if (a == "a")   {return 96;}
  else if (a == "213") {return 16;}

 return 24;
}

int main()
{
int k = 10;
printf("%d\n", k--);
cmdLine a;
a.inst = "25 if z > a goto 213";
a.expand();
a.print();

cmdLine c;
c.inst = "25 if d <= a goto 213";
c.expand();
c.print();

cmdLine b;
b.inst = "16 let y = d * ( x / d )";
b.expand();
b.print();
return 0;
}
