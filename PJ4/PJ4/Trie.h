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

template <typename ValueType>
class Trie
{
public:
    Trie() {}
    ~Trie() { delete m_root; }
    void reset()
    {
        delete m_root;
        m_root = new Node;
    }
    void insert(const string &key, const ValueType &value)
    {
        m_root->insert(key, value);
    }
    vector<ValueType> find(const string &key, bool exactMatchOnly) const
    {
        return exactMatchOnly ?
            m_root->findExact(key) : m_root->findSNiP(key, true);
    }
    // C++11 syntax for preventing copying and assignment
    Trie(const Trie &) = delete;
    Trie &operator=(const Trie &) = delete;

private:
    class Node
    {
    public:
        Node() {}
        Node(const char &label) : m_label(label) {}
        ~Node()
        {
            for (auto const &child : m_children)
                if (child != nullptr)
                    delete child;
        }
        Node *getChild(const char &ch) const
        {
            for (auto const &child : m_children)
                if (child->m_label == ch)
                    return child;
            return nullptr;
        }
        void insert(const string &key, const ValueType &value)
        {
            if (key.empty())
            {
                m_values.push_back(value);
                return;
            }
            Node *child = getChild(key[0]);
            if (child == nullptr)
            {
                child = new Node(key[0]);
                m_children.push_back(child);
            }
            child->insert(key.substr(1), value);
        }
        vector<ValueType> findExact(const string &key) const
        {
            if (key.empty())
                return m_values;
            Node * const next = getChild(key[0]);
            if (next == nullptr)
                return vector<ValueType>();
            else
                return next->findExact(key.substr(1));
        }
        vector<ValueType> findSNiP(const string &key, const bool &first) const
        {
            if (key.empty())
                return m_values;
            vector<ValueType> result;
            Node * const next = getChild(key[0]);
            for (auto const &child : m_children)
            {
                vector<ValueType> found;
                if (child == next) {
                    // exact match on this char, still can do SNiP next time.
                    found = child->findSNiP(key.substr(1), false);
                }
                else if (!first) {
                    // SNiP mismatch here; must exact match from now on.
                    found = child->findExact(key.substr(1));
                }
                result.insert(result.end(), found.begin(), found.end());
            }
            return result;
        }
    private:
        char m_label;
        vector<ValueType> m_values;
        vector<Node *> m_children;
    };
    Node *m_root = new Node;
};

#endif /* Trie_h */
