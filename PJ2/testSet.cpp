/*
 * testSet.cpp
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
  string tmp;

  cerr << endl;
  cerr << "This test assumes that ItemType is set to `std::string`." << endl;

  test_log("TEST 1: Constructor test", '=');
  Set s1;
  assert(s1.size() == 0);
  assert(s1.empty());
  test_log("TEST 1: Passed!", '=');
  cerr << endl;

  test_log("TEST 2: Insert Tests", '=');
  test_log("PART 1: duplicate insert test", '-');
  Set s2;
  assert(s2.insert("1"));
  assert(s2.size() == 1);
  test_log("inserting duplicating string", ' ');
  assert(! s2.insert("1"));
  // The insertion was not successful, the size should not be altered.
  assert(s2.size() == 1);
  test_log("passed!", ' ');
  test_log("PART 2: bulk insert test", '-');
  Set s3;
  // insert 250 items (essentially strings of number 1 to 250)
  for (int i = 1; i <= 250; i += 1) {
    if (i % 50 == 0)
      test_log("trying inserting " + to_string(i) + " ...", ' ');
    assert(s3.insert(to_string(i)));
    assert(s3.size() == i);
    if (i % 50 == 0)
      test_log("checking contains " + to_string(i) + " ...", ' ');
    assert(s3.contains(to_string(i)));
  }
  assert(s3.contains(to_string(250)));
  test_log("passed!", ' ');
  test_log("TEST 2: Passed!", '=');
  cerr << endl;

  test_log("TEST 3: Erase Tests", '=');
  test_log("PART 1: size consistancy test", '-');
  Set s4;
  assert(s4.insert("1"));  // size() == 1
  assert(! s4.insert("1"));  // size() == 1
  assert(s4.insert("2"));  // size() == 2
  assert(s4.insert("3"));  // size() == 3
  assert(s4.insert("4"));  // size() == 4
  assert(s4.erase("4"));  // size() == 3
  test_log("checking if erase reduces size correctly...", ' ');
  assert(s4.size() == 3);
  assert(!s4.erase("4"));
  // "4" is no longer part of s4, this shouldn't change size
  assert(s4.size() == 3);
  assert(s4.erase("1"));  // size() == 2
  assert(s4.size() == 2);
  assert(!s4.erase("1"));
  // "1" is no longer part of s4, this shouldn't change size
  assert(s4.size() == 2);
  test_log("passed!", ' ');
  test_log("PART 2: underloading test", '-');
  assert(s4.erase("2"));  // size() == 1
  assert(s4.erase("3"));  // size() == 0
  assert(s4.empty());  // the set is empty
  assert(s4.size() == 0);
  // double check if s4 still contains the items that should've been removed
  assert(! s4.contains("1"));
  assert(! s4.contains("2"));
  // keep trying to erase when the set is already empty
  assert(! s4.erase("1"));
  assert(! s4.erase("2"));
  assert(! s4.erase("3"));
  assert(! s4.erase("4"));
  // double check if s4 still contains the items that should've been removed
  assert(! s4.contains("3"));
  assert(! s4.contains("4"));
  // make sure the size of the set is still 0, but not something negative
  assert(s4.empty());
  assert(s4.size() == 0);
  test_log("passed!", ' ');
  test_log("TEST 3: Passed!", '=');
  cerr << endl;

  test_log("TEST 4: Get Test", '=');
  test_log("PART 1: false alteration test", '-');
  Set s5;
  tmp = "don't touch me";
  assert(! s5.get(0, tmp));
  assert(tmp.compare("don't touch me") == 0);
  test_log("passed!", ' ');
  test_log("PART 2: ordering test", '-');
  assert(s5.insert("3"));  // 3
  assert(s5.insert("8"));  // 3 -> 8
  assert(s5.insert("2"));  // 2 -> 3 -> 8
  assert(! s5.insert("3"));  // 2 -> 3 -> 8
  assert(s5.insert("4"));  // 2 -> 3 -> 4 -> 8
  assert(s5.size() == 4);
  assert(s5.get(0, tmp));
  assert(tmp.compare("2") == 0);
  assert(s5.get(1, tmp));
  assert(tmp.compare("3") == 0);
  assert(s5.get(2, tmp));
  assert(tmp.compare("4") == 0);
  assert(s5.get(3, tmp));  // nothing is at index 4.
  assert(tmp.compare("8") == 0);
  assert(! s5.get(4, tmp));  // nothing is at index 4.
  assert(tmp.compare("8") == 0);
  assert(s5.contains("2"));
  assert(s5.contains("3"));
  assert(s5.contains("4"));
  assert(s5.contains("8"));
  test_log("passed!", ' ');
  test_log("PART 2: actual copy test", '-');
  // This part is intended to test if the set insert a copy of the value, rather
  // than the original value. This could cause problem if the original value is 
  // altered after insertion. For example, the ordering of the set may be 
  // broken.
  Set s6;
  string *str_do = new string;
  *str_do = "do";
  string *str_love = new string;
  *str_love = "love";
  string *str_me = new string;
  *str_me = "me";
  s6.insert(*str_do);  // "do"
  s6.insert(*str_love);  // "do" -> "love"
  s6.insert(*str_me);  // "do" -> "love" -> "me"
  // Now we will try to alter the content of the set from the outside.
  *str_do = "love";
  *str_love = "me";
  *str_me = "do";
  // (Elvis Presley liked this test.)
  // Now we check if s6 is altered.
  string recv;
  assert(s6.get(0, recv));
  assert(recv.compare("do") == 0);
  assert(s6.get(1, recv));
  assert(recv.compare("love") == 0);
  assert(s6.get(2, recv));
  assert(recv.compare("me") == 0);
  test_log("passed!", ' ');
  delete str_do;
  delete str_love;
  delete str_me;
  test_log("TEST 4: Passed!", '=');
  cerr << endl;

  test_log("TEST 5: Swap test", '=');
  // Reusing s5 and s4 from TEST 3
  // s4 is empty, s5 is 2 -> 3 -> 4 -> 8
  s5.swap(s4);
  assert(s5.size() == 0);
  tmp = "nope";
  assert(! s5.get(0, tmp));
  assert(tmp == "nope");
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
  assert(! s4.get(4, tmp));
  assert(tmp == "8");
  test_log("TEST 5: Passed!", '=');
  cerr << endl;

  test_log("TEST 6: Assignment Operator", '=');
  Set assigner, assignee;
  test_log("PART 1: memory exchange test", '-');
  assigner.insert("1");
  assignee = assigner;
  // both assignee and assigner should contain "1" now
  assert(! assignee.insert("1"));  // duplicate "1", can't insert
  assert(assigner.erase("1"));
  // destruct the original "1", the new "1" shouldn't be affected
  assert(! assignee.insert("1"));  // duplicate "1", still can't insert
  // assignee should still contain that "1"
  assert(assignee.contains("1"));
  assert(assignee.insert("2"));
  assert(assignee.insert("3"));
  assert(assignee.insert("4"));
  assert(assignee.insert("5"));
  assert(assignee.insert("6"));
  assert(assignee.insert("7"));
  assert(assignee.insert("8"));
  assert(assignee.insert("9"));
  assert(assigner.insert("1"));
  test_log("passed!", ' ');
  test_log("PART 2: deep copy test", '-');
  tmp = "no!";
  assert(assignee.get(0, tmp));
  assert(tmp.compare("1") == 0);
  assert(assignee.get(8, tmp));
  assert(tmp.compare("9") == 0);
  // should be checking an invalid index
  assert(! assignee.get(9, tmp));
  // tmp's value shouldn't change
  assert(tmp.compare("9") == 0);
  // make sure assigner and assignee are not sharing memory
  assert(! assigner.contains("3"));
  assert(assignee.contains("1"));
  assert(assignee.erase("1"));
  assert(assigner.contains("1"));
  test_log("passed!", ' ');
  test_log("TEST 6: Passed!", '=');
  cerr << endl;

  test_log("TEST 7 Copy constructor", '=');
  Set toclone;
  toclone.insert("10");  // toclone: 10
  toclone.insert("11");  // toclone: 10 -> 11
  Set cloned = toclone;  // cloned: 10 -> 11
  // make sure the copy process is a deep copy
  assert(cloned.erase("10"));  // cloned: 11
  assert(toclone.get(0, tmp));  // toclone[0] should still be 10
  assert(tmp.compare("10") == 0);
  assert(toclone.size() == 2);
  assert(cloned.size() == 1);
  assert(cloned.contains("11"));
  assert(! cloned.contains("10"));
  test_log("TEST 7: Passed!", '=');
  cerr << endl;

  test_log("TEST 8 unite and subtract", '=');
  test_log("PART 1: self-assigning unite test", '-');
  // Make sure when the result is one of the operand set, unite still works.
  Set implosion1, implosion2;
  implosion1.insert("i");
  implosion1.insert("m");
  implosion1.insert("p");
  implosion1.insert("l");
  implosion1.insert("o");
  implosion1.insert("s");
  implosion2.insert("i");
  implosion2.insert("o");
  implosion2.insert("n");
  unite(implosion1, implosion2, implosion1);
  string tassu;
  assert(implosion1.get(0, tassu));
  assert(tassu.compare("i") == 0);
  assert(implosion1.get(1, tassu));
  assert(tassu.compare("l") == 0);
  assert(implosion1.get(2, tassu));
  assert(tassu.compare("m") == 0);
  assert(implosion1.get(3, tassu));
  assert(tassu.compare("n") == 0);
  assert(implosion1.get(4, tassu));
  assert(tassu.compare("o") == 0);
  assert(implosion1.get(5, tassu));
  assert(tassu.compare("p") == 0);
  assert(implosion1.get(6, tassu));
  assert(tassu.compare("s") == 0);
  assert(! implosion1.get(7, tassu));
  assert(tassu.compare("s") == 0);
  test_log("passed!", ' ');
  test_log("PART 2: self-assigning subtract test", '-');
  // Make sure when the result is one of the operand set, subtract still works.
  Set explosion1, explosion2;
  explosion1.insert("e");
  explosion1.insert("x");
  explosion1.insert("p");
  explosion1.insert("l");
  explosion1.insert("o");
  explosion1.insert("s");
  explosion2.insert("i");
  explosion2.insert("o");
  explosion2.insert("n");
  subtract(explosion1, explosion2, explosion2);
  string tasss;
  assert(explosion2.get(0, tasss));
  assert(tasss.compare("e") == 0);
  assert(explosion2.get(1, tasss));
  assert(tasss.compare("l") == 0);
  assert(explosion2.get(2, tasss));
  assert(tasss.compare("p") == 0);
  assert(explosion2.get(3, tasss));
  assert(tasss.compare("s") == 0);
  assert(explosion2.get(4, tasss));
  assert(tasss.compare("x") == 0);
  assert(! explosion2.get(5, tasss));
  assert(tasss.compare("x") == 0);
  test_log("passed!", ' ');
  test_log("PART 3: unite functionality test", '-');
  // Make sure unite works as expected
  Set su1;
  su1.insert("12");
  Set su2 = su1;
  su2.insert("11");
  su2.insert("13");
  su2.insert("12");
  su2.insert("15");
  su1.insert("10");
  su1.insert("14");
  su1.insert("16");
  Set united;
  united.insert("no");
  united.insert("not");
  united.insert("me");
  united.insert("10");
  united.insert("11");
  united.insert("12");
  unite(su1, su2, united);
  test_log("Checking if su1 is changed after unite (it shouldn't) ...", ' ');
  assert(su1.size() == 4);
  string tu = "Hmmm";
  assert(su1.get(0, tu));
  assert(tu.compare("10") == 0);
  assert(su1.get(1, tu));
  assert(tu.compare("12") == 0);
  assert(su1.get(2, tu));
  assert(tu.compare("14") == 0);
  assert(su1.get(3, tu));
  assert(tu.compare("16") == 0);
  test_log("Checking if su2 is changed after unite (it shouldn't) ...", ' ');
  assert(su2.size() == 4);
  assert(su2.get(0, tu));
  assert(tu.compare("11") == 0);
  assert(su2.get(1, tu));
  assert(tu.compare("12") == 0);
  assert(su2.get(2, tu));
  assert(tu.compare("13") == 0);
  assert(su2.get(3, tu));
  assert(tu.compare("15") == 0);
  test_log("Checking if the result is correct", ' ');
  assert(united.size() == 7);
  assert(united.get(0, tu));
  assert(tu == "10");
  assert(united.get(1, tu));
  assert(tu == "11");
  assert(united.get(2, tu));
  assert(tu == "12");
  assert(united.get(3, tu));
  assert(tu == "13");
  assert(united.get(4, tu));
  assert(tu == "14");
  assert(united.get(5, tu));
  assert(tu == "15");
  assert(united.get(6, tu));
  assert(tu == "16");
  assert(! united.get(7, tu));
  assert(tu == "16");
  test_log("passed!", ' ');
  test_log("PART 4: subtract functionality test", '-');
  // Make sure subtract works as expected
  Set ss1;
  ss1.insert("12");
  Set ss2 = ss1;
  ss2.insert("11");
  ss2.insert("13");
  ss2.insert("12");
  ss2.insert("15");
  ss1.insert("10");
  ss1.insert("14");
  ss1.insert("16");
  Set subtracted;
  subtracted.insert("no");
  subtracted.insert("not");
  subtracted.insert("me");
  subtracted.insert("10");
  subtracted.insert("11");
  subtracted.insert("12");
  subtract(ss1, ss2, subtracted);
  test_log("Checking if ss1 is changed after subtract (it shouldn't) ...", ' ');
  assert(ss1.size() == 4);
  string ts = "Hmmm";
  assert(ss1.get(0, ts));
  assert(ts.compare("10") == 0);
  assert(ss1.get(1, ts));
  assert(ts.compare("12") == 0);
  assert(ss1.get(2, ts));
  assert(ts.compare("14") == 0);
  assert(ss1.get(3, ts));
  assert(ts.compare("16") == 0);
  test_log("Checking if ss2 is changed after subtract (it shouldn't) ...", ' ');
  assert(ss2.size() == 4);
  assert(ss2.get(0, ts));
  assert(ts.compare("11") == 0);
  assert(ss2.get(1, ts));
  assert(ts.compare("12") == 0);
  assert(ss2.get(2, ts));
  assert(ts.compare("13") == 0);
  assert(ss2.get(3, ts));
  assert(ts.compare("15") == 0);
  test_log("Checking if the result is correct", ' ');
  assert(subtracted.size() == 3);
  assert(subtracted.get(0, ts));
  assert(ts == "10");
  assert(subtracted.get(1, ts));
  assert(ts == "14");
  assert(subtracted.get(2, ts));
  assert(ts == "16");
  assert(! subtracted.get(3, ts));
  assert(ts == "16");
  test_log("passed!", ' ');
  test_log("TEST 8: Passed!", '=');
  cerr << endl;

  test_log("TEST 9: Dump Test");
  united.dump();
  subtracted.dump();

  test_log("All tests passed!", '*');
  cerr << endl;
}
