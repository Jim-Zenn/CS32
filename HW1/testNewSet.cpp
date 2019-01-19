/*
 * main.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include "newSet.h"
#include <cassert>
#include <iostream>
#include <string>

using namespace std;

void test_log(const string &msg, const char &sep = ' ') {
  cerr << sep << sep << sep << " " << msg << " " << sep << sep << sep << endl;
}

int main() {
  cerr << endl;

  test_log("TEST 1: Set::Set()", '=');
  Set s0;
  Set s7(1);
  test_log("TEST 1: Passed!", '=');
  cerr << endl;

  test_log("TEST 2: int Set::size() const", '=');
  Set s1;
  assert(s1.size() == 0);
  assert(s1.empty());
  test_log("TEST 2: Passed!", '=');
  cerr << endl;

  test_log("TEST 3: bool Set::insert(const ItemType& value)", '=');
  test_log("PART 1: duplicate and size alteration test", ' ');
  Set s2;
  assert(s2.insert("1"));
  assert(s2.size() == 1);
  assert(!s2.insert("1")); // 1 already present in set, should return false.
  assert(s2.size() == 1);
  test_log("passed!", ' ');
  test_log("PART 2: overloading test", ' ');
  Set s3;
  for (int i = 0; i < DEFAULT_MAX_ITEMS; i += 1) {
    assert(s3.insert(to_string(i)));
    assert(s3.size() == i + 1);
    assert(s3.contains(to_string(i)));
  }
  assert(!s3.insert("999")); // The set is full, should return false.
  assert(!s3.contains("999"));
  assert(s3.contains(to_string(DEFAULT_MAX_ITEMS - 1)));
  test_log("passed!", ' ');
  test_log("TEST 3: Passed!", '=');
  cerr << endl;

  // bool Set::erase(const ItemType& value)
  test_log("TEST 4: bool Set::erase(const ItemType& value)", '=');
  test_log("PART 1: size alteration test", ' ');
  Set s4;
  assert(s4.insert("1"));
  assert(!s4.insert("1"));
  assert(s4.insert("2"));
  assert(s4.insert("3"));
  assert(s4.insert("4"));
  assert(s4.erase("4"));
  assert(s4.size() == 3);
  assert(!s4.erase("4"));
  assert(s4.size() == 3);
  assert(s4.erase("1"));
  assert(s4.size() == 2);
  assert(!s4.erase("1"));
  assert(s4.size() == 2);
  test_log("passed!", ' ');
  test_log("PART 2: underloading test", ' ');
  assert(s4.erase("2"));
  assert(s4.erase("3"));
  assert(s4.empty());
  assert(s4.size() == 0);
  assert(!s4.contains("1"));
  assert(!s4.contains("2"));
  assert(!s4.erase("1"));
  assert(!s4.erase("2"));
  assert(!s4.erase("3"));
  assert(!s4.erase("4"));
  assert(!s4.contains("3"));
  assert(!s4.contains("4"));
  assert(s4.empty());
  assert(s4.size() == 0);
  test_log("passed!", ' ');
  test_log("TEST 4: Passed!", '=');
  cerr << endl;

  test_log("TEST 5: bool get(int i, ItemType& value) const", '=');
  test_log("PART 1: null alteration test", ' ');
  Set s5;
  string tmp = "100";
  assert(!s5.get(0, tmp));
  assert(tmp == "100");
  test_log("passed!", ' ');
  test_log("PART 2: ordering test", ' ');
  assert(s5.insert("3"));
  assert(s5.insert("8"));
  assert(s5.insert("2"));
  assert(!s5.insert("3"));
  assert(s5.insert("4"));
  assert(s5.size() == 4);
  assert(s5.get(0, tmp));
  assert(tmp == "2");
  assert(s5.get(1, tmp));
  assert(tmp == "3");
  assert(s5.get(2, tmp));
  assert(tmp == "4");
  assert(s5.get(3, tmp));
  assert(tmp == "8");
  assert(!s5.get(4, tmp));
  assert(tmp == "8");
  assert(s5.contains("2"));
  assert(s5.contains("3"));
  assert(s5.contains("4"));
  assert(s5.contains("8"));
  test_log("passed!", ' ');
  test_log("TEST 5: Passed!", '=');
  cerr << endl;

  test_log("TEST 6: void swap(Set& other)", '=');
  test_log("PART 1: swapping different sized sets test", ' ');
  s5.swap(s4);
  assert(s5.size() == 0);
  assert(!s5.get(0, tmp));
  assert(tmp == "8");
  assert(s4.size() == 4);
  assert(s4.contains("2"));
  assert(s4.contains("3"));
  assert(s4.contains("4"));
  assert(s4.contains("8"));
  assert(s4.get(0, tmp));
  assert(tmp == "2");
  assert(s4.get(1, tmp));
  assert(tmp == "3");
  assert(s4.get(2, tmp));
  assert(tmp == "4");
  assert(s4.get(3, tmp));
  assert(tmp == "8");
  assert(!s4.get(4, tmp));
  assert(tmp == "8");
  test_log("passed!", ' ');
  test_log("PART 2: swapping different capacity sets test", ' ');
  Set s8(2);
  Set s9(1);
  s8.swap(s9);
  assert(s8.insert("1"));
  assert(!s8.insert("2"));
  assert(s9.insert("2"));
  assert(s9.insert("3"));
  assert(!s9.insert("4"));
  test_log("passed!", ' ');
  test_log("TEST 6: Passed!", '=');
  cerr << endl;

  test_log("All tests passed!", '*');
  cerr << endl;
}
