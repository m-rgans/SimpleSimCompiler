//***************************************************************************
//
//  mystack.h
//  CSCI 241 Assignment 7
//
//  Created by Alex Peterson (z1912480) and Joshua Sulouff (z1867688)
//
//*************************
#ifndef MYSTACK_H
#define MYSTACK_H

#include <iostream>

class mystack
{
private:

    char* stk_array = nullptr;
    size_t stk_capacity = 0;
    size_t stk_size = 0;

public:

    mystack() = default;
    mystack(const mystack& x);
    ~mystack();
    mystack& operator=(const mystack& x);
    size_t capacity() const;
    size_t size() const;
    bool empty() const;
    void clear();
    void reserve(size_t n);
    const char& top() const;
    void push(char value);
    void pop();
};

#endif
