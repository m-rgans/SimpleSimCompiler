//***************************************************************************
//
//  mystack.cpp
//  CSCI 241 Assignment 7
//
//  Created by Alex Peterson (z1912480) and Joshua Sulouff (z1867688)
//
//*************************
#include "mystack.h"
#include <cstring>

using namespace std;

//void *memcpy(void *dest, const void * src, size_t n)

/**
 * copy constructor for mystack
 *
 * @param const mystack& x - base object to copy from
 * @return none
 ********************************************************************/
mystack::mystack(const mystack& x)
{
  *this = x;
  if (stk_capacity == 0)
  {stk_array = nullptr;}

}

/**
 * destructor for mystack
 *
 *
 * @return none
 ********************************************************************/
mystack::~mystack()
{
    delete[] stk_array;
}

/**
 * = operator overload for mystack
 *
 * @param const mystack& x - base object to copy from
 * @return pointer to self
 ********************************************************************/
mystack& mystack::operator=(const mystack& x)
{
    reserve(x.stk_capacity);
    stk_size = x.stk_size;
    if (!(stk_array == nullptr || x.stk_array == nullptr))
    {
    memcpy(stk_array, x.stk_array, stk_size * sizeof(char));
    }
    return *this;
}

/**
 * get the allocated space of a mystack
 *
 *
 * @return unsigned long int representing number of slots allocated for
 ********************************************************************/
size_t mystack::capacity() const
{
    return stk_capacity;
}

/**
 * retrieve the used space of the stack
 *
 *
 * @return number of slots assigned to a value that have not been popped off
 ********************************************************************/
size_t mystack::size() const
{
    return stk_size;
}

/**
 * check if the stack is empty
 *
 *
 * @return whether the stack is empty
 ********************************************************************/
bool mystack::empty() const
{
    return (stk_size == 0);
}

/**
 * Clear the contents of the stack
 *
 *
 * @return none
 ********************************************************************/
void mystack::clear()
{
    stk_size = 0;

}
/**
 * allocate a certain amount of space for the stack
 *
 * @param - size_t n - amount of slots to allocate
 * @return none
 ********************************************************************/
void mystack::reserve(size_t n)
{
  if (n < stk_capacity || n == stk_capacity)
  {
    return;
  }

  char* temp = new char[n];

  if (!(stk_array==nullptr)) {
    memcpy(temp, stk_array, stk_size * sizeof(char));
  }


  delete[] stk_array;
  stk_array = temp;
  stk_capacity = n;

}

/**
 * retrieve the object on top of the stack
 *
 *
 * @return reference to top object of stack
 ********************************************************************/
const char& mystack::top() const
{

    return stk_array[stk_size - 1];
}

/**
 * Append value to top of stack
 *
 *  @param char value - the value to be pushed to the stack
 *  @return none
 ********************************************************************/
void mystack::push(char value)
{
  if (stk_capacity == 0)
  {
    reserve(1);
  }
  else if (stk_size >= stk_capacity) {
    reserve(stk_capacity * 2);
  }

  stk_array[stk_size] = value;
  stk_size++;
}

/**
 * delete top item from stack
 *
 *
 * @return none
 ********************************************************************/
void mystack::pop()
{
  if ((stk_array == nullptr)) {
    return;
  }
    stk_size--;
}
