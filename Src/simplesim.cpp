
//***************************************************************************
//
//  simplesim.cpp
//  CSCI 241 Assignment 4
//
//  Created by Alex Peterson (z1912480) and Joshua Sulouff (z1867688)
//
//***************************************************************************

#include "simplesim.h"
#include "sml.h"
#include <cstdio>
#include <iostream>
#include <iomanip>

#define WMAX 9999

#define RMAX 100

void printword(int);
void printword(int, int);

  //Enum of crash types
  enum panic_code
  {
    LOAD_TOO_LARGE,
    LOAD_WORD_INVALID,

    BAD_OPCODE,

    BAD_ADDRESS,
    DIV_0,

    UNDERFLOW,
    OVERFLOW,
    READ_WORD_INVALID,
  };

/**
 * Called when an error occurs that requires the sim to stop.
 * @param int errCode - Error code used in switch statement to print appropriate abend
 * @return nothing
 ********************************************************************************/
void simplesim::panic(int errCode)
{
    //print the associated error message
    switch (errCode) {
    case LOAD_TOO_LARGE:
      fprintf(stderr, "*** ABEND: pgm load: pgm too large ***\n\n");
      break;
    case LOAD_WORD_INVALID:
      fprintf(stderr, "*** ABEND: pgm load: invalid word ***\n\n");
      break;
    case BAD_OPCODE:
      fprintf(stderr, "*** ABEND: invalid opcode ***\n\n");
      break;
    case BAD_ADDRESS:
      fprintf(stderr, "*** ABEND: addressability error ***\n\n");
      break;
    case DIV_0:
      fprintf(stderr, "*** ABEND: attempted division by 0 ***\n\n");
      break;
    case UNDERFLOW:
      fprintf(stderr, "*** ABEND: underflow ***\n\n");
      break;
    case OVERFLOW:
      fprintf(stderr, "*** ABEND: overflow ***\n\n");
      break;
    case READ_WORD_INVALID:
      fprintf(stderr, "*** ABEND: illegal input ***\n\n");
      break;
    }
    //stop emulation.
    stop = true;
}
/**
 * Read a value from cin into memory.
 * @param - int addr - The address is the position in memory we are writing to from cin
 * @return nothing
 ********************************************************************************/
void simplesim::read(int addr)
{
  //create a storage value and pull into it from cin
  int a;
  std::cin >> a;

  //make sure that the value isn't out of range
  if (a > WMAX || a < -WMAX) {
    panic(READ_WORD_INVALID);
    return;
  }

  //print the value and store it
  std::cout << "READ: ";
  printword(a);
  std::cout << '\n';
  ram[addr] = a;
}
/**
 * print a value in memory to console
 * @param int addr - The address is the position in memory we are printing out to the console
 * @return nothing
 ********************************************************************************/
void simplesim::write(int addr)
{
  //check for addressability
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }

  //print the number at address
  int value = ram[addr];
  printword(value);
  std::cout << '\n';
}
/**
 * store a value from memory into the accumulator
 * @param int addr - The current position in memory we are storing the accumulator value to
 * @return nothing
 ********************************************************************************/
void simplesim::store(int addr)
{
  //check for addressability
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }
  //
  ram[addr] = accumulator;
  return;
}
/**
 * load a value from memory into the accumulator
 * @param int addr - The current position in memory to be stored into the accumulator
 * @return nothing
 ********************************************************************************/
void simplesim::load(int addr)
{
  //check for addressability
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }
  accumulator = ram[addr];
}
/**
 * add a value in memory to the value in the accumulator
 * @param int addr - The current position in memory of the number we are adding to the accumulator
 * @return nothing
 ********************************************************************************/
void simplesim::add(int addr)
{
  //check address in bounds
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }

  //Check for over/underflows
  int result = accumulator + ram[addr];
  if (result > WMAX) {
    panic(OVERFLOW);
    return;
  }
  else if (result < -WMAX)
  {
    panic(UNDERFLOW);
    return;
  }

  accumulator = result;
}
/**
 * subtract a value from the value in the accumulator
 * @param int addr - The current position in memory of the number we are subracting from the accumulator
 * @return nothing
 ********************************************************************************/
void simplesim::subtract(int addr)
{
  //check for addressability
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }

  //Check for over/underflows
  int result = accumulator - ram[addr];
  if (result > WMAX) {
    panic(OVERFLOW);
    return;
  }
  else if (result < -WMAX)
  {
    panic(UNDERFLOW);
    return;
  }

  accumulator = result;
}
/**
 * cause the program counter to jump to a specific position in memory
 * @param int addr - Program counter is set to current address and branches to that point in memory
 * @return nothing
 * @note  - decrements by one due to function being at the bottom of while loop
 ********************************************************************************/
void simplesim::branch(int addr)
{
  //check for addressability
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }
  pc = addr - 1;
  return;
}
/**
 * Send the pc to a different position if the accumulator holding zero
 * @param int addr - Program counter is set to addr if accumulator is zero
 * @return nothing
 * @note  - decrements by one due to function being at the bottom of while loop
 ********************************************************************************/
void simplesim::branchzero(int addr)
{
  //check for addressability
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }
  if (accumulator == 0) {
    pc = addr - 1;
  }

}
/**
 * send execution to a different position in memory if accumulator is negative
 * @param int addr - Program counter is set to addr if accumulator is negative
 * @return nothing
 * @note  - decrements by one due to function being at the bottom of while loop
 ********************************************************************************/
void simplesim::branchneg(int addr)
{
  //check for addressability
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }

  if (accumulator < 0) {
    pc = addr - 1;
  }

}
/**
 * multiply the accumulator by the value at addr
 * @param int addr - Multply accumulator by number at address
 * @return nothing
 ********************************************************************************/
void simplesim::mult(int addr)
{
  //check for addressability
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }

  //check result
  int result = accumulator * ram[addr];
  if (result > WMAX) {
    panic(OVERFLOW);
    return;
  }
  else if (result < -WMAX)
  {
    panic(UNDERFLOW);
    return;
  }
  accumulator = result;
}
/**
 * divide the accumulator by a value in memory
 * @param int addr - Divide accumulator by number at address
 * @return nothing
 ********************************************************************************/
void simplesim::div(int addr)
{
  //check for addressability
  if (addr > RMAX || addr < 0) {
    panic(BAD_ADDRESS);
    return;
  }

  //check for /0 error
  if (ram[addr] == 0) {
    panic(DIV_0);
    return;
  }

  //check result value
  int result = accumulator / ram[addr];
  if (result > WMAX) {
    panic(OVERFLOW);
    return;
  }
  else if (result < -WMAX)
  {
    panic(UNDERFLOW);
    return;
  }
  //commit
  accumulator = result;
}
/**
 * Set up instruction opcode and operand registers
 * @param int word - The 4 digit word command to be split into opcode and operand registers
 * @return nothing
 ********************************************************************************/
void simplesim::divbyte(int word)
{
  //get high and low "bytes" of current instruction
  inst = word;
  int high = word / 100;
  int low = word % 100;
  opcode = high;
  operand = low;
}
/**
 * Simplesim constructor
 * Initialize all registers
 * @return nothing
 ********************************************************************************/
simplesim::simplesim()
{
  //set defaults
  accumulator = 0;
  pc = 0;
  inst = 0;
  opcode = 0;
  operand = 0;
  stop = false;
}
/**
 * Reads input file from cin into ram to prepare for execution
 * @return bool to be used in main to move to execute function
 ********************************************************************************/
bool simplesim::load_program()
{
  const int TERMINATE = -99999;
  const int BLANK = 7777;

  //clear memory
  for (size_t i = 0; i < 100; i++) {
    ram[i] = BLANK;
  }


  int pop;
  int address = 0;



  //pop things off of cin until we hit the terminator
  while (std::cin >> pop) {

    //check for out of memory
    if (address > RMAX)
    {
      panic(LOAD_TOO_LARGE);
      break;
    }

    //check for eof
    if (pop == TERMINATE)
      {
        break;
      }

    //check size
    if (pop > WMAX || pop < -WMAX)
    {
      panic(LOAD_WORD_INVALID);
      break;
    }




      //put into next place
      ram[address] = pop;
      pop++;
      address++;
  }

  if (pop != TERMINATE) {
    //panic: no terminator
    return false;
  }


  return true;

}
/**
 * Starts executing instructions at ram address zero
 * based on current opcode
 ********************************************************************************/
void simplesim::execute_program()
{
  //set pc to -1 so that first increment sets it to start
  pc = -1;
  opcode = 0;
  accumulator = 0;
  inst = 0;
  operand = 0;

  //if we crashed during load, increment the pc so that its not -1 when dumping
  if (stop) {
    pc++;
  }


  //execute instruction
  while (!stop) {
    //increment to next instruction
    pc++;

    //make sure the pc doesn't run off the memory
    if (pc >= RMAX || pc < 0) {
      panic(BAD_ADDRESS);
      break;
    }

    //setup opcode and operand
    divbyte(ram[pc]);

    switch (opcode) {
      case READ:
        read(operand);
        break;
      case WRITE:
        write(operand);
        break;

      case STORE:
        store(operand);
        break;

      case LOAD:
        load(operand);
        break;

      case ADD:
        add(operand);
        break;
      case SUBTRACT:
        subtract(operand);
        break;
      case MULTIPLY:
        mult(operand);
        break;
      case DIVIDE:
        div(operand);
        break;

      case BRANCH:
        branch(operand);
        break;
      case BRANCHZERO:
        branchzero(operand);
        break;
      case BRANCHNEG:
        branchneg(operand);
        break;
      case HALT:
        printf("*** Simplesim execution terminated ***\n\n");
        stop = true;
        break;
      default:
        panic(BAD_OPCODE);
        break;
    }


  }
}


/**
 * Print registers and memory to console
 ********************************************************************************/
void simplesim::dump()
{
  //number of memory positions per line
  const int PERLINE = 10;

  printf("REGISTERS:\n");

  printf("accumulator:            ");
  printword(accumulator);
  printf("\n");

  printf("instruction_counter:    ");
  printword(pc, 2);
  printf("\n");

  printf("instruction_register:   ");
  printword(inst);
  printf("\n");

  printf("operation_code:         ");
  printword(opcode, 2);
  printf("\n");

  printf("operand:                ");
  printword(operand, 2);
  printf("\n\nMEMORY:\n");



  printf("       0     1     2     3     4     5     6     7     8     9\n");

  //get number of lines to print
  int lines = RMAX/PERLINE;
  //push over 1 space for 0
  std::cout << " ";

  //for each line...
  for (int i = 0; i < (lines); i++) {
    //print the line number
    printf("%d ", (i * PERLINE));
    //for each index on the line
    for (int c = 0; c < PERLINE; c++) {
      //print the number and a space if we are not at the end of the line
      printword(ram[c + i*PERLINE]);
      if (!(c == PERLINE - 1)) {
        std::cout << " ";
      }
    }
    //go to next line
    std::cout << '\n';
  }

}

/**
 * @param int a - the word command to be printed
 * @return nothing
 ********************************************************************************/
void printword(int a)
{
  //print the sign
  if (a >= 0)
  {std::cout << "+";}
  else
  {
    std::cout << "-";
    a *= -1;
  }

  //get digits from sprintf and dump result into temp
  const int width = 4;
  char temp[11] = "%d";
  int digits = sprintf(temp, "%d" ,a);
  int extra = width - digits;



  //add leading zeros
  for (int i = 0; i < extra; i++) {
    std::cout << "0";
  }
  //print value
  std::cout << a;
}

/**
 * @param int a - the word command to be printed
 * @param int width - used by second printword function in the case of items that don't have
 * signs
 * @return nothing
 ********************************************************************************/
void printword(int a, int width)
{

  //create a temporary array for sprintf to dump its result into
  char temp[10] = "%d";
  int digits = sprintf(temp, "%d" ,a);
  int extra = width - digits;



  //add leading zeros as needed
  for (int i = 0; i < extra; i++) {
    std::cout << "0";
  }
  std::cout << a;
}
