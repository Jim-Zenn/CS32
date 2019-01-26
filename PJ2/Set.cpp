/*
 * Set.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <cassert>

#include "Set.h"

using namespace std;

Set::Set(const Set &src) : m_sentinel(new Node), m_size(src.size())
{
  Node *src_curr = src.m_sentinel->next;
  Node *curr = m_sentinel;

  while (src_curr != src.m_sentinel) {
    Node *node = new Node;
    node->value = src_curr->value;
    node->prev = curr;
    node->next = m_sentinel;
    curr->next = node;
    m_sentinel->prev = node;

    curr = node;
    src_curr = src_curr->next;
  }
}

Set::~Set() 
{
  Node *curr = m_sentinel->next;
  Node *next;
  while (curr != m_sentinel) {
    next = curr->next;
    delete curr;
    curr = next;
  }
  delete m_sentinel;
}

Set &Set::operator= (const Set &src)
// Copy all the nodes from the src and assign to this.
{
  // first remove all nodes
  Node *curr = m_sentinel->next;
  Node *next;
  while (curr != m_sentinel) {
    next = curr->next;
    delete curr;
    curr = next;
  }
  Node *src_curr = src.m_sentinel->next;
  curr = m_sentinel;

  // copy nodes over from src.
  while (src_curr != src.m_sentinel) {
    Node *node = new Node;
    node->value = src_curr->value;
    node->prev = curr;
    node->next = m_sentinel;
    curr->next = node;
    m_sentinel->prev = node;

    curr = node;
    src_curr = src_curr->next;
  }
  m_size = src.size();

  return *this;
}

bool Set::insert(const ItemType &value) {
  Node *curr = m_sentinel->next;
  while (curr != m_sentinel && curr->value <= value) {
    if (curr->value == value)
      // it is already in the set
      return false;
    curr = curr->next;
  }
  Node *node = new Node;
  node->value = value;
  // linking
  node->prev = curr->prev;
  node->next = curr;
  node->prev->next = node;
  curr->prev = node;
  m_size += 1;
  return true;
}

bool Set::erase(const ItemType &value) {
  Node *curr = findNode(value);
  if (curr == m_sentinel)
    // Not found
    return false;
  // Found. Now remove the node.
  curr->prev->next = curr->next;
  curr->next->prev = curr->prev;
  delete curr;
  m_size -= 1;
  return true;
}

bool Set::contains(const ItemType &value) const {
  Node *curr = findNode(value);
  return curr != m_sentinel;
}

bool Set::get(int i, ItemType &receiver) const {
  if (i < 0 || i >= size())
    return false;
  Node *curr = m_sentinel;
  for (int j = 0; j <= i; j += 1) {
    curr = curr->next;
  }
  receiver = curr->value;
  return true;
}

void Set::swap(Set &other) {
  // swap dllist
  Node *tmp_node;
  tmp_node = m_sentinel;
  m_sentinel = other.m_sentinel;
  other.m_sentinel = tmp_node;

  // swap size
  int tmp_int;
  tmp_int = m_size;
  m_size = other.m_size;
  other.m_size = tmp_int;
}

void Set::dump() const {
  Node *curr = m_sentinel;
  do {
    curr = m_sentinel->next;
    cout << curr->value << endl;
  } while (curr != m_sentinel);
  cerr << endl;
}
