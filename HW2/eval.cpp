/*
 * eval.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>
#include <cctype>
#include <cassert>

#include "Set.h"

using namespace std;

int opPrecedence(const char &op);
// returns the priority of the given operator
// If the given character is not an operator, the program will quit

int parse(const string &infix, string &postfix);
// parse an infix boolean expression to a postfix boolean expression
// return 0 if parse was successful; otherwise, return an integer error code

int evaluate(string infix, const Set& trueValues, const Set& falseValues,
             string& postfix, bool& result);

int opPrecedence(const char &op)
{
  switch (op) {
    case '!':  return 3;
    case '&':  return 2;
    case '|':  return 1;
    default:   exit(1);  // error: op is not a operator, quitting
  }
}

int parse(const string &infix, string &postfix)
{
  postfix = "";  // set postfix to empty
  int state = 0;  // start with state 0
  int pc = 0;  // parentheses counter
  stack<char> operatorStack;
  for (char ch : infix) {
    switch(ch) {
      // operand
      case 'a' ... 'z':
        if (state == 1)  return 1;  // should be at state 0
        postfix += ch;
        state = 1;  // update state
        break;
      // left parenthesis
      case '(':
        if (state == 1)  return 2;  // should be at state 0
        pc += 1;  // update pc
        state = 0;  // update state
        operatorStack.push('(');  // push '(' to the stack
        break;
      // right parenthesis
      case ')':
        if (state == 0)  return 3;  // should be at state 1
        pc -= 1;  // update pc
        if (pc < 0)  return 4;  // pc should never be negative
        state = 1;  //update state
        // pop until the previous '('
        while (operatorStack.top() != '(') {
          postfix += operatorStack.top();
          operatorStack.pop();
        }
        operatorStack.pop();  // pop the '('
        break;
      // binary operators
      case '&': case '|':
        if (state == 0)  return 5;  // should be at state 1
        state = 0;
        // pop operators with equal or higher priority
        while (!operatorStack.empty() && operatorStack.top() != '(' &&
               opPrecedence(ch) <= opPrecedence(operatorStack.top())) {
          postfix += operatorStack.top();
          operatorStack.pop();
        }
        operatorStack.push(ch);
        break;
      // unary operator
      case '!':
        // since the unary operator '!' has the absolute highest priority
        // no operators should be popped before it.
        // note that latter '!' has a de facto higher priority than earlier '!'
        operatorStack.push(ch);
        break;
      // whitespaces
      case ' ':
        break;
      // illegal characters
      default:
        return 7;
    }
  }
  if (state == 0)  return 8;  // should not end at state 0
  if (pc != 0)  return 9;  // the number of parentheses should match
  // pop empty the rest of the stack
  while (! operatorStack.empty()) {
    postfix += operatorStack.top();
    operatorStack.pop();
  }
  return 0;
}

int evaluate(string infix, const Set& trueValues, const Set& falseValues,
             string& postfix, bool& result) {
  int parseRtn = parse(infix, postfix);
  if (parseRtn != 0)  // parse error
    return 1;
  // start evaluating the postfix expression
  stack<bool> operandStack;
  bool operand1, operand2;
  for (char ch : postfix) {
    bool isTrue, isFalse;
    switch(ch) {
      // operand
      case 'a' ... 'z':
        isTrue = trueValues.contains(ch);
        isFalse = falseValues.contains(ch);
        if (!isTrue && !isFalse)  return 2;  // Neither true nor false
        if (isTrue && isFalse)    return 3;  // both true and false
        if (isTrue)   operandStack.push(true);
        if (isFalse)  operandStack.push(false);
        break;
      // binary operators
      case '|':
      case '&':
        operand1 = operandStack.top();
        operandStack.pop();
        operand2 = operandStack.top();
        operandStack.pop();
        if (ch == '|')
          operandStack.push(operand1 || operand2);
        else if (ch == '&')
          operandStack.push(operand1 && operand2);
        break;
      // unary operator
      case '!':
        operand1 = operandStack.top();
        operandStack.pop();
        operandStack.push(!operand1);
        break;
    }
  }
  result = operandStack.top();
  return 0;
}

int main()
{
    string trueChars  = "abcd";
    string falseChars = "wxyz";
    Set trues;
    Set falses;
    for (int k = 0; k < trueChars.size(); k++)
        trues.insert(trueChars[k]);
    for (int k = 0; k < falseChars.size(); k++)
        falses.insert(falseChars[k]);

    string postfix;
    bool result;

    // basic cases
    assert(evaluate("a|w", trues, falses, postfix, result) == 0);
    assert(postfix == "aw|");
    assert(result == true);
    assert(evaluate("!a", trues, falses, postfix, result) == 0);
    assert(postfix == "a!");
    assert(result == false);
    assert(evaluate("!!!!a", trues, falses, postfix, result) == 0);
    assert(postfix == "a!!!!");
    assert(result == true);
    assert(evaluate("w&a", trues, falses, postfix, result) == 0);
    assert(postfix == "wa&");
    assert(result == false);
    assert(evaluate("(a)", trues, falses, postfix, result) == 0);
    assert(postfix == "a");
    assert(result == true);
    assert(evaluate("((a))", trues, falses, postfix, result) == 0);
    assert(postfix == "a");
    assert(result == true);
    // error cases
    assert(evaluate("", trues, falses, postfix, result) == 1);
    assert(evaluate("!@#$%^&*()", trues, falses, postfix, result) == 1);
    assert(evaluate("()", trues, falses, postfix, result) == 1);
    assert(evaluate(")a((", trues, falses, postfix, result) == 1);
    assert(evaluate("))a(", trues, falses, postfix, result) == 1);
    assert(evaluate(")a(", trues, falses, postfix, result) == 1);
    assert(evaluate("&a|w", trues, falses, postfix, result) == 1);
    assert(evaluate("a&a|", trues, falses, postfix, result) == 1);
    assert(evaluate("!&a", trues, falses, postfix, result) == 1);
    assert(evaluate("a&&a", trues, falses, postfix, result) == 1);
    assert(evaluate("b|&b", trues, falses, postfix, result) == 1);
    assert(evaluate("c||c", trues, falses, postfix, result) == 1);
    assert(evaluate("cc", trues, falses, postfix, result) == 1);
    assert(evaluate("c!!c", trues, falses, postfix, result) == 1);
    assert(evaluate("(c)c", trues, falses, postfix, result) == 1);
    assert(evaluate("(c)!c", trues, falses, postfix, result) == 1);
    assert(evaluate("(!c)!c", trues, falses, postfix, result) == 1);
    assert(evaluate("c(c)", trues, falses, postfix, result) == 1);
    assert(evaluate("c(!c)", trues, falses, postfix, result) == 1);
    // operator precedence check
    assert(evaluate("a|b&!c    ", trues, falses, postfix, result) == 0);
    assert(postfix == "abc!&|");
    assert(result == true);
    // general correctness check
    assert(evaluate("( b |  w ) & ! c ", trues, falses, postfix, result) == 0);
    assert(postfix == "bw|c!&");
    assert(result == false);
    assert(evaluate("    b&! (w|!c)", trues, falses, postfix, result) == 0);
    assert(postfix == "bwc!|!&");
    assert(result == true);
    assert(evaluate("!(!(!(a)&b)|c)&w", trues, falses, postfix, result) == 0);
    assert(postfix == "a!b&!c|!w&");
    assert(result == false);

    cout << "Passed all tests" << endl;

    return 0;
}
