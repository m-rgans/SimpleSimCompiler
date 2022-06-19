//***************************************************************************
//
//  sml.hpp
//  CSCI 241 Assignment 4
//
//  Created by Alex Peterson (z1912480) and Joshua Sulouff (z1867688)
//
//***************************************************************************

#ifndef FILE_SML
#define FILE_SML ;

enum INSTRUCTION
{
  READ  = 11,
  WRITE = 12,

  STORE = 21,
  LOAD  = 22,

  ADD   = 31,
  SUBTRACT = 32,
  MULTIPLY = 33,
  DIVIDE   = 34,

  BRANCH   = 41,
  GOTO     = 41,
  BRANCHZERO = 42,
  BRANCHNEG = 43,
  HALT = 44,
};

#endif /* end of include guard: FILE_SML */
