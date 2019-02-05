/*
 * newSet.h
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef NEWSET_H
#define NEWSET_H

#include <string>

const int DEFAULT_MAX_ITEMS = 250;

/* #define ITEMTYPE_UNSIGNED_LONG */

#ifdef ITEMTYPE_UNSIGNED_LONG
using ItemType = unsigned long;
#else
using ItemType = std::string;
#endif

class Set {
public:
  Set(const int &capacity = DEFAULT_MAX_ITEMS);
  // Create an with given capcacity, otherwise with default
  // capacity.

  Set(const Set &orig);
  // Copy the orig set.

  ~Set();
  // Destruct the set.

  Set &operator=(const Set &src);
  // Assignment operator.

  bool empty() const { return m_size == 0; };
  // Return true if the set is empty, otherwise false.

  int size() const { return m_size; }
  // Return the number of items in the set.

  bool insert(const ItemType &value);
  // Insert value into the set if it is not already present.  Return
  // true if the value was actually inserted.  Leave the set unchanged
  // and return false if the value was not inserted (perhaps because it
  // was already in the set or because the set has a fixed capacity and
  // is full).

  bool erase(const ItemType &value);
  // Remove the value from the set if present.  Return true if the
  // value was removed; otherwise, leave the set unchanged and
  // return false.

  bool contains(const ItemType &value) const;
  // Return true if the value is in the set, otherwise false.

  bool get(int i, ItemType &value) const;
  // If 0 <= i < size(), copy into value the item in the set that is
  // strictly greater than exactly i items in the set and return true.
  // Otherwise, leave value unchanged and return false.

  void swap(Set &other);
  // Exchange the contents of this set with the other one.

  void dump() const;
  // Writes the items in the array to stderr.

private:
  bool is_full() { return m_size >= m_capacity; };
  // Return whether the set is full.

  int m_capacity;
  int m_size;
  ItemType **m_array;
};

#endif /* !NEWSET_H */
