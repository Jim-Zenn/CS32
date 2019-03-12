#ifndef BST_H
#define BST_H

class BST
{
  private:
    class Node
    {
      public:
        Node(Node *parent, int value)
            : lchild(nullptr), rchild(nullptr), parent(parent), value(value) {}
        int value;
        Node *lchild;
        Node *rchild;
        Node *parent;
    };
    Node *m_root;
  public:
    BST(): m_root(nullptr) {}
    void insert(int const &value) {
        m_root = _insert(m_root, nullptr, value);
    }
    Node *_insert(Node *node, Node * const &parent, int const &value)
    {
        if (node == nullptr)
            return new Node(parent, value);
        if (value < node->value)
            node->lchild = _insert(node->lchild, node, value);
        if (value > node->value)
            node->rchild = _insert(node->rchild, node, value);
        return node;
    }
};

#endif
