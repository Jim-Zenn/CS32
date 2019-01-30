/*
 * Set.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>

#include "Set.h"

using namespace std;

Set::Set() : m_guard(new Node), m_size(0)
// Create an empty set (i.e., one with no items).
{
  // initialize the circular linked list.
  m_guard->next = m_guard;
  m_guard->prev = m_guard;
}

Set::Set(const Set &src) : Set()
{
  // iterate through the source set
  for (Node *sp = src.m_guard->next; sp != src.m_guard; sp = sp->next) {
    // for each node of the source set, insert a node with the same value and 
    // append it to this set.
    insertNodeBefore(sp->val, m_guard);
  }
}

Set::~Set()
{
  clear();  // clear the whole set
  delete m_guard;  // remove the guard node
}

Set &Set::operator=(const Set &src)
{
  if (&src == this)
    // the source is this set itself
    return *this;
  clear();  // clear the whole set
  // iterate through the source set
  for (Node *sp = src.m_guard->next; sp != src.m_guard; sp = sp->next) {
    // for each node of the source set, insert a node with the same value and 
    // append it to this set.
    insertNodeBefore(sp->val, m_guard);
  }
  return *this;
}

bool Set::insert(const ItemType &val)
{
  Node *p;
  // iterate through the set to find the right spot
  for (p = m_guard->next; p != m_guard; p = p->next) {
    if (p->val == val)
      // the given value is already in this set
      return false;
    if (p->val > val)
      break;
  }
  // found the spot to insert the new node
  insertNodeBefore(val, p);
  return true;
}

bool Set::erase(const ItemType &val)
{
  // If the given value is not found in the set, `getNodeWith` would return the
  // guard node, which signify's "not found".  In this case, `removeNode` would
  // do nothing and return false.  Otherwise, `getNodeWith` would return the
  // node with the given value, and `removeNode` would remove this node and
  // return true.
  return removeNode(getNodeWith(val));
}

bool Set::contains(const ItemType &val) const
{
  // If the given value is not found in the set, `getNodeWith` would return the
  // guard node, which signify's "not found".
  return getNodeWith(val) != m_guard;
}

bool Set::get(int i, ItemType &val) const
{
  if (i < 0 || i >= size())
    // the given index is not valid
    return false;
  Node *p = m_guard->next;
  for (; i > 0; i -= 1) 
    p = p->next;
  val = p->val;
  return true;
}

void Set::swap(Set &other)
{
  // swap the guard nodes and all the nodes that precedes them
  Node *tmp_node;
  tmp_node = m_guard;
  m_guard = other.m_guard;
  other.m_guard = tmp_node;

  // swap size
  int tmp_int;
  tmp_int = m_size;
  m_size = other.m_size;
  other.m_size = tmp_int;
}

void Set::dump() const
{
  for (Node *p = m_guard->next; p != m_guard; p = p->next)
    cerr << p->val << " ";
  cerr << endl;
}

void Set::clear() {
  Node *next;
  for (Node *p = m_guard->next; p != m_guard; p = next) {
    next = p->next;
    removeNode(p);
  }
}

Set::Node *Set::getNodeWith(const ItemType &val) const
{
  for (Node *p = m_guard->next; p != m_guard; p = p->next) {
    if (p->val == val)
      // Found it
      return p;
  }
  return m_guard;
}

bool Set::removeNode(Node *node) {
  if (node == m_guard)
    // the given node is the guard node
    return false;
  // detach the node from the linked list
  node->prev->next = node->next;
  node->next->prev = node->prev;
  // destruct the node
  delete node;
  // reduce the size of the set by 1
  m_size -= 1;
  return true;
}

void Set::insertNodeAfter(const ItemType &val, Node *p) 
{
  Node *node = new Node;
  node->val = val;
  // link the node's prev and next pointer first
  node->prev = p;
  node->next = p->next;
  // link node's prev's next and its next's prev to itself
  node->prev->next = node;
  node->next->prev = node;
  // increase the size of the set by 1
  m_size += 1;
}

void Set::insertNodeBefore(const ItemType &val, Node *p) 
{
  // This is essentially the same as insertNodeAfter, except that this function
  // insert the new node before the given positioning node.
  insertNodeAfter(val, p->prev);
}

void unite(const Set &s1, const Set &s2, Set &result)
{
  Set r;
  ItemType tmp;
  // try to insert all s1's elements into the result
  for (int i = 0; i < s1.size(); i += 1) {
    s1.get(i, tmp);
    r.insert(tmp);
  }
  // try to insert all s2's elements into the result
  for (int i = 0; i < s2.size(); i += 1) {
    s2.get(i, tmp);
    r.insert(tmp);
  }
  result = r;
}

void subtract(const Set &s1, const Set &s2, Set &result)
{
  Set r;
  ItemType tmp;
  // try to insert all s1's elements into the result
  for (int i = 0; i < s1.size(); i += 1) {
    s1.get(i, tmp);
    r.insert(tmp);
  }
  // try to remove all elements that appears in from the result
  for (int i = 0; i < s2.size(); i += 1) {
    s2.get(i, tmp);
    r.erase(tmp);
  }
  result = r;
}
