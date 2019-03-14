//
//  Trie.h
//  PJ4
//
//  Created by Jim Zenn on 3/9/19.
//  Copyright © 2019 UCLA. All rights reserved.
//

#ifndef Trie_h
#define Trie_h

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename V> class Trie {
public:
  Trie() {}
  ~Trie() { delete m_root; }
  void reset() {
    delete m_root;           // delete the nodes recursively
    m_root = new Node('\0'); // create a new node
  }
  void insert(const string &key, const V &value) { m_root->insert(key, value); }
  vector<V> find(const string &key, bool exactMatchOnly) const {
    return exactMatchOnly ? m_root->findExact(key)
                          : m_root->findSNiP(key, true);
  }
  // C++11 syntax for preventing copying and assignment
  Trie(const Trie &) = delete;
  Trie &operator=(const Trie &) = delete;

private:
  class Node {
  public:
    Node(const char &label) : m_label(label) {}
    ~Node() {
      // destruct all child nodes first
      for (auto const &child : m_children)
        delete child;
    }
    // get the child with the given label. If none exist, return nullptr.
    Node *getChild(const char &label) const {
      for (auto const &child : m_children)
        if (child->m_label == label)
          return child;
      return nullptr;
    }
    // add the given value to this node
    void add(const V &value) { m_values.push_back(value); }
    // insert the given value to the sub trie rooted with this node
    void insert(const string &key, const V &value);
    // return the values at the node indexed exactly by the given key
    vector<V> findExact(const string &key) const;
    // return the values at the node indexed by the given key with one mismatch
    vector<V> findSNiP(const string &key, const bool &first) const;

  private:
    char const m_label;        // this node's key label, should be const
    vector<V> m_values;        // the values stored in this node
    vector<Node *> m_children; // the child nodes of this node
  };

  Node *m_root = new Node('\0'); // the root node of the trie
};

template <typename V>
void Trie<V>::Node::insert(const string &key, const V &value) {
  if (key.empty()) {
    add(value);
    return;
  }
  char const keyLabel = key[0];
  // find on which child should insert be performed
  Node *child = getChild(keyLabel);
  if (child == nullptr) {
    // no child with the given label is found
    // create new child with this label
    child = new Node(keyLabel);
    m_children.push_back(child);
  }
  child->insert(key.substr(1), value);
}

template <typename V>
vector<V> Trie<V>::Node::findExact(const string &key) const {
  if (key.empty())
    return m_values;
  char const keyLabel = key[0];
  Node *const next = getChild(keyLabel);
  // if the child with the label does not exist, then no value corresponds
  // with the given key; otherwise, continue to search the sub trie
  return next == nullptr ? vector<V>() : next->findExact(key.substr(1));
}

template <typename V>
vector<V> Trie<V>::Node::findSNiP(const string &key, const bool &first) const {
  if (key.empty())
    return m_values;
  vector<V> result;
  char const keyLabel = key[0];
  for (auto const &child : m_children) {
    vector<V> found;
    if (child->m_label == keyLabel)
      // exact match on this char, still can do SNiP next time.
      found = child->findSNiP(key.substr(1), false);
    else if (!first)
      // SNiP mismatch here; must exact match from now on.
      found = child->findExact(key.substr(1));
    result.insert(result.end(), found.begin(), found.end());
  }
  return result;
}

#endif /* Trie_h */
