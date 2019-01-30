/*
 * Set.h
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SET_H
#define SET_H

#include <string>

using ItemType = std::string;

class Set {
public:
  Set();
  // Create an empty set (i.e., one with no items).

  Set(const Set &src);
  // Create an copy of the src set.

  ~Set();
  // Remove all the nodes in the set, including the sentinel node.

  Set &operator= (const Set &src);
  // Copy all the nodes from the src and assign to this.

  bool empty() const { return size() == 0; };
  // Return true if the set is empty, otherwise false.

  int size() const { return m_size; }
  // Return the number of items in the set.

  bool insert(const ItemType &val);
  // Insert value into the set if not already present.  Return true if the value
  // was actually inserted; otherwise, since the value must be already present,
  // return false.

  bool erase(const ItemType &val);
  // Remove the value from the set if present.  Return true if the value was 
  // removed; otherwise, leave the set unchanged and return false.

  bool contains(const ItemType &val) const;
  // Return true if the value is in the set, otherwise false.

  bool get(int i, ItemType &val) const;
  // If 0 <= i < size(), copy into value the item in the set that is strictly 
  // greater than exactly i items in the set and return true. Otherwise, leave 
  // value unchanged and return false.

  void swap(Set &other);
  // Exchange the contents of this set with the other one.

  void dump() const;
  // Writes the items in the array to cerr.

private:
  struct Node {
    ItemType val;
    Node *prev, *next;
  };

  Node *m_guard;
  int m_size;

  void clear();
  // Remove all nodes except the guard node in the set. Note that it does not 
  // remove the guard node; it only resets the set to the initial status.

  Node *getNodeWith(const ItemType &val) const;
  // Return the node in the set with the given value.  If a node with the given
  // value exists, return the pointer to that node; otherwise, if no node with 
  // the value is found, return the guard node instead.

  bool removeNode(Node *node);
  // Remove the given node.  If the given node is the guard node, return false;
  // otherwise, remove and destruct the node, reduce the set's size by 1, and 
  // return true.

  void insertNodeAfter(const ItemType &val, Node *pos);
  // Create a new node with the given value, insert the new node after the
  // positioning node with proper linking, and increase the set's size by 1.

  void insertNodeBefore(const ItemType &val, Node *pos);
  // Create a new node with the given value, insert the new node before the
  // positioning node with proper linking, and increase the set's size by 1.
};

void unite(const Set& s1, const Set& s2, Set& result);
// Returns result set that contains one copy of each of the values that appear 
// in s1 or s2 or both. This function does not assume result is empty. Hence it
// will reset the result at the beginning.

void subtract(const Set& s1, const Set& s2, Set& result);
// Returns result set that contains one copy of each of the values that appear 
// in s1 but not s2. This function does not assume result is empty. Hence it
// will reset the result at the beginning.

#endif /* !SET_H */
