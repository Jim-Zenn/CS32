/*
 * SSNSet.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>

#include "SSNSet.h"
#include "Set.h"

using namespace std;

void SSNSet::print() const {
  unsigned long tmp;
  int i = 0;
  while (m_set.get(i++, tmp))
    cout << tmp << endl;
}
