/*
 * Set.h
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SET_H
#define SET_H

#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

/* #define ITEMTYPE_UNSIGNED_LONG */

#ifdef ITEMTYPE_UNSIGNED_LONG
using ItemType = unsigned long;
#else
using ItemType = string;
#endif

class Set {
public:
  Set() : m_sentinel(new Node), m_size(0)
  // Create an empty set (i.e., one with no items).
  {
    m_sentinel->next = m_sentinel;
    m_sentinel->prev = m_sentinel;
  }

  Set(const Set &src);
  // Create an copy of the src set.

  ~Set();
  // Remove all the nodes in the set, including the sentinel node.

  Set &operator= (const Set &src);
  // Copy all the nodes from the src and assign to this.

  bool empty() { return size() == 0; };
  // Return true if the set is empty, otherwise false.

  int size() const { return m_size; }
  // Return the number of items in the set.

  bool insert(const ItemType &value);
  // Insert value into the set if it is not already present.  Return
  // true if the value was actually inserted.  Leave the set unchanged
  // and return false if the value was not inserted (perhaps because it
  // was already in the set).

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
 
  struct Node {
    ItemType value;
    Node *prev, *next;
  };

  Node *m_sentinel;
  int m_size;

  Node *findNode(const ItemType &query) const
  // find the first node in the DLList that contains the query value.
  // Returns the reference to that node if found; otherwise, return reference
  // to the sentinel node.
  {
    Node *curr = m_sentinel;
    do {
      curr = curr->next;
      if (curr->value == query)
        // Found it
        return curr;
    } while (curr != m_sentinel);
    return m_sentinel;
  }

};

#endif /* !SET_H */
