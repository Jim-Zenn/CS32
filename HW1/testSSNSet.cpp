/*
 * testSSNSet.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include "SSNSet.h"

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

void test_log(const string &msg, const char &sep = ' ') {
  cerr << sep << sep << sep << " " << msg << " " << sep << sep << sep << endl;
}

int main() {
  cerr << endl;

  test_log("TEST 1: SSNSet::SSNSet()", '=');
  SSNSet s;
  test_log("TEST 1: Passed!", '=');
  cerr << endl;

  test_log("TEST 2: SSNSet::add(unsigned long ssn)", '=');
  SSNSet s1;
  assert(s1.add(1));
  assert(s1.add(2));
  assert(!s1.add(2));
  test_log("TEST 2: Passed!", '=');
  cerr << endl;

  test_log("TEST 3: SSNSet::size()", '=');
  assert(s1.size() == 2);
  test_log("TEST 3: Passed!", '=');
  cerr << endl;

  s1.print();
  cerr << endl;

  test_log("All tests passed!", '*');
  cerr << endl;
}
