/*
 * main.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include "Set.h"
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
  for (int i = 0; i < 250; i += 1) {
    if (i % 50 == 0)
      test_log("inserting " + to_string(i), ' ');
    assert(s3.insert(to_string(i)));
    assert(s3.size() == i + 1);
    if (i % 50 == 0)
      test_log("checking contains " + to_string(i), ' ');
    assert(s3.contains(to_string(i)));
  }
  test_log("passed the loop!", ' ');
  assert(s3.contains(to_string(249)));
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
  test_log("TEST 6: Passed!", '=');
  cerr << endl;

  test_log("TEST 7 Set Set::&operator= (const Set &old) [Assignment Op]", '=');
  Set assigner, assignee;
  test_log("PART 1: memory exchanging test", ' ');
  assigner.insert("1");
  assignee = assigner;  // assignment operator called here
  assert(! assignee.insert("1"));
  assert(assignee.insert("2"));
  assert(assignee.insert("3"));
  assert(assignee.insert("4"));
  assert(assignee.insert("5"));
  assert(assignee.insert("6"));
  assert(assignee.insert("7"));
  assert(assignee.insert("8"));
  assert(assignee.insert("9"));
  test_log("passed!", ' ');
  test_log("PART 2: deep copy test", ' ');
  ItemType t2 = "no!";
  assert(assignee.get(0, t2));
  cout << t2 << endl;
  assert(t2.compare("1") == 0);
  cout << assignee.size() << endl;
  assert(assignee.get(8, t2));
  cout << t2 << endl;
  assert(t2.compare("9") == 0);
  assert(! assignee.get(9, t2));  // should be checking an invalid index
  assert(t2.compare("9") == 0);  // t2's value shouldn't change at this point
  assert(! assigner.contains("3"));  // make sure they are not sharing memory!
  assert(assignee.contains("1"));
  assert(assignee.erase("1"));
  assert(assigner.contains("1"));
  test_log("passed!", ' ');
  test_log("TEST 7: Passed!", '=');
  cerr << endl;

  test_log("TEST 8 Set::Set(const Set &old) [Copy constuctor]", '=');
  Set s16;
  s16.insert("10");
  s16.insert("11");
  Set s17 = s16;  // copy constructor called here
  assert(s17.erase("10"));
  ItemType t;
  assert(s16.get(0, t));
  assert(t.compare("11"));
  assert(s16.size() == 2);
  assert(s17.size() == 1);
  assert(s17.contains("11"));
  assert(!s17.contains("10"));
  test_log("TEST 8: Passed!", '=');
  cerr << endl;

  test_log("TEST 9 unite and subtract", '=');
  test_log("PART 1: unite functionality test", ' ');
  Set s18;
  s18.insert("12");
  Set s19 = s18;  // copy constructor called here
  s19.insert("11");
  s18.insert("10");
  Set united;
  unite(s18, s19, united);
  assert(united.size() == 3);
  ItemType t3;
  assert(united.get(0, t3));
  assert(t3 == "10");
  assert(united.get(1, t3));
  assert(t3 == "11");
  assert(united.get(2, t3));
  assert(t3 == "12");
  assert(! united.get(3, t3));
  assert(t3 == "12");
  test_log("passed!", ' ');
  test_log("PART 2: subtract functionality test", ' ');
  Set subtracted;
  subtract(s18, s19, subtracted);
  assert(subtracted.size() == 1);
  assert(subtracted.get(0, t3));
  assert(t3 == "10");
  assert(! subtracted.get(1, t3));
  assert(t3 == "10");
  test_log("passed!", ' ');
  test_log("TEST 9: Passed!", '=');
  cerr << endl;

  test_log("All tests passed!", '*');
  cerr << endl;
}

