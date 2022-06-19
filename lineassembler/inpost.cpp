//***************************************************************************
//
//  inpost.cpp
//  CSCI 241 Assignment 7
//
//  Created by Alex Peterson (z1912480) and Joshua Sulouff (z1867688)
//
//*************************
#include <string>
#include <cctype>
#include "mystack.h"
#include "inpost.h"

using std::string;

/**
 * Translates a string expression in infix notation to postfix notation
 *
 * @param const string& infix - the string to be converted.
 * @return string in postfix notation
 ********************************************************************/
string convert(const string& infix)
{
    string postfix;
    mystack s;

    unsigned int ndx = 0;

    while (ndx < infix.length())
    {
        //variable
        if (islower(infix[ndx]))
        {
            postfix += infix[ndx];
            postfix += ' ';
            ndx++;
            continue;
        }

        //literal
        else if (isdigit(infix[ndx]))
        {
            while (isdigit(infix[ndx]))
            {
                //keep appending and dont put a space after it until done
                postfix += infix[ndx];
                ndx++;
            }
            postfix += ' ';
            continue;
        }

        //space
        else if (isspace(infix[ndx]))
        {

          ndx++;
          continue;
        }

        //left paren
        else if (infix[ndx] == '(')
        {
            s.push(infix[ndx]);
            ndx++;

            continue;
        }
        //right paren
        else if (infix[ndx] == ')')
        {
          while (s.size())
          {
            if (s.top() == '(') {
              break;
            }
            postfix += s.top();
            s.pop();
            postfix += ' ';
          }
          if (s.size()) {
            s.pop();
          }
          ndx++;
        }
        //operator
        else
        {
          while (s.size()) {
            if (!(precedence(infix[ndx]) <= precedence(s.top()))) {
              break;
            }
            postfix += s.top();
            postfix += ' ';

            s.pop();
          }
          s.push(infix[ndx]);
          ndx++;
        }
    }

    while (!s.empty())
    {
      postfix += s.top();
      postfix += ' ';
      s.pop();
    }

    postfix.pop_back();
    return postfix;

}

/**
 * Handles order of operations in infix notation to give a priority for given character
 *
 * @param char op - the operator to be compared
 * @return integer representing priority
 ********************************************************************/
int precedence(char op)
{

    switch (op)
    {
        case '~':
        case '^':
            return 3;
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return -1;
    }

}
