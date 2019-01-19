/*
 * Set.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>

#include "Set.h"

using namespace std;

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
    if (m_array[i] > value) {
      // found the spot
      break;
    }
  }
  // now everybody at the right of spot
  // (including the one in the spot), move!
  for (int j = size(); j > i; j -= 1) {
    m_array[j] = m_array[j - 1];
  }
  // insert the new item
  m_array[i] = ItemType(value);
  m_size += 1;
  return true;
}

bool Set::erase(const ItemType &value) {
  int i;
  for (i = 0; i < size(); i += 1) {
    if (m_array[i] == value)
      // found the value
      break;
  }
  if (i == size())
    // value not present
    return false;
  m_size -= 1;
  for (; i < size(); i += 1) {
    m_array[i] = m_array[i + 1];
  }
  return true;
}

bool Set::contains(const ItemType &value) const {
  for (int i = 0; i < size(); i += 1) {
    if (m_array[i] == value)
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
  value = ItemType(m_array[i]);
  return true;
}

void Set::swap(Set &other) {
  // swap array
  int min_size = size() < other.size() ? size() : other.size();
  int i;
  for (i = 0; i < min_size; i += 1) {
    ItemType tmp_item = m_array[i];
    m_array[i] = other.m_array[i];
    other.m_array[i] = tmp_item;
  }
  if (size() < other.size()) {
    for (; i < other.size(); i += 1) {
      m_array[i] = other.m_array[i];
    }
  } else {
    for (; i < size(); i += 1) {
      other.m_array[i] = m_array[i];
    }
  }
  int tmp;
  // swap size
  tmp = m_size;
  m_size = other.m_size;
  other.m_size = tmp;
}

void Set::dump() const {
  for (int i = 0; i < size(); i += 1) {
    cerr << m_array[i] << " ";
  }
  cerr << endl;
}
