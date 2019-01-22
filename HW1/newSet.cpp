/*
 * newSet.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdlib>
#include <iostream>

#include "newSet.h"

using namespace std;

Set::Set(const int &capacity)
    : m_capacity(capacity), m_size(0), m_array(new ItemType *[m_capacity]) {
  if (capacity < 0) {
    cout << "A set cannot have negative capacity." << endl;
    exit(1);
  }
}

Set::Set(const Set &orig)
    : m_capacity(orig.m_capacity), m_size(orig.m_size),
      m_array(new ItemType *[m_capacity]) {
  for (int i = 0; i < orig.size(); i++)
    m_array[i] = new ItemType(*orig.m_array[i]);
}

Set::~Set() {
  for (int i = 0; i < size(); i += 1)
    delete m_array[i];
  delete[] m_array;
};

bool Set::insert(const ItemType &value) {
  if (contains(value))
    // the value is already present in the set
    return false;
  if (is_full())
    // the set is already full
    return false;
  // find the spot for the new value
  int i;
  for (i = 0; i < size(); i += 1) {
    if (*m_array[i] > value) {
      // found the spot
      break;
    }
  }
  // now everybody to the right, move!
  for (int j = size(); j > i; j -= 1) {
    m_array[j] = m_array[j - 1];
  }
  // insert the new item
  m_array[i] = new ItemType(value);
  m_size += 1;
  return true;
}

bool Set::erase(const ItemType &value) {
  int i;
  for (i = 0; i < size(); i += 1) {
    if (*m_array[i] == value)
      // found the value
      break;
  }
  if (i == size())
    // value not present
    return false;
  m_size -= 1;
  delete m_array[i];
  for (; i < size(); i += 1) {
    m_array[i] = m_array[i + 1];
  }
  return true;
}

bool Set::contains(const ItemType &value) const {
  for (int i = 0; i < size(); i += 1) {
    if (*m_array[i] == value)
      // found the value
      return true;
  }
  // Not found
  return false;
}

bool Set::get(int i, ItemType &value) const {
  if (i < 0 || i >= size())
    // out of range
    return false;
  value = *m_array[i];
  return true;
}

void Set::swap(Set &other) {
  // swap array
  ItemType **tmp_ptr = m_array;
  m_array = other.m_array;
  other.m_array = tmp_ptr;
  int tmp;
  // swap size
  tmp = m_size;
  m_size = other.m_size;
  other.m_size = tmp;
  // swap capacity
  tmp = m_capacity;
  m_capacity = other.m_capacity;
  other.m_capacity = tmp;
}
