#include <iostream>

using namespace std;

struct Node
{
    int data;
    int height;
    Node *antecessor;
    Node *rightChild;
    Node *leftChild;
};

class AVLBinaryTree
{
private:
    Node *root_;

public:
    AVLBinaryTree(int data) : root_(newNode(data)) {}

    Node *root() const { return root_; }

    void add(int data)
    {
        Node *node = newNode(data);
        root_ = insert(root_, node);
    }

    Node *find(int data)
    {
        Node *auxNode = root_;
        while (auxNode != nullptr && auxNode->data != data)
        {
            if (data < auxNode->data)
            {
                auxNode = auxNode->leftChild;
            }
            else
            {
                auxNode = auxNode->rightChild;
            }
        }
        return auxNode;
    }

    void remove(int data)
    {
        Node *node = find(data);
        if (node != nullptr)
        {
            root_ = delNode(root_, node);
        }
    }

    static Node *delNode(Node *tree, Node *node)
    {
        if (tree == nullptr)
        {
            return tree;
        }

        if (node->data < tree->data)
        {
            tree->leftChild = delNode(tree->leftChild, node);
        }
        else if (node->data > tree->data)
        {
            tree->rightChild = delNode(tree->rightChild, node);
        }
        else
        {
            if (tree->leftChild == nullptr)
            {
                Node *temp = tree->rightChild;
                delete tree;
                return temp;
            }
            else if (tree->rightChild == nullptr)
            {
                Node *temp = tree->leftChild;
                delete tree;
                return temp;
            }

            Node *temp = minValueNode(tree->rightChild);
            tree->data = temp->data;
            tree->rightChild = delNode(tree->rightChild, temp);
        }

        tree->height = 1 + max(height(tree->leftChild), height(tree->rightChild));

        int balance = getBalance(tree);
        if (balance > 1)
        {
            if (getBalance(tree->leftChild) >= 0)
            {
                tree = rightRotate(tree);
            }
            else
            {
                tree->leftChild = leftRotate(tree->leftChild);
                tree = rightRotate(tree);
            }
        }
        else if (balance < -1)
        {
            if (getBalance(tree->rightChild) <= 0)
            {
                tree = leftRotate(tree);
            }
            else
            {
                tree->rightChild = rightRotate(tree->rightChild);
                tree = leftRotate(tree);
            }
        }

        return tree;
    }

    static int height(Node *node)
    {
        if (node == nullptr)
        {
            return -1; // Assuming leaf nodes have height 0
        }
        return node->height;
    }

    static int getBalance(Node *tree)
    {
        if (tree == nullptr)
        {
            return 0;
        }
        return height(tree->leftChild) - height(tree->rightChild);
    }

    static Node *insert(Node *tree, Node *node)
    {
        if (tree == nullptr)
        {
            return node;
        }

        if (node->data < tree->data)
        {
            tree->leftChild = insert(tree->leftChild, node);
        }
        else if (node->data > tree->data)
        {
            tree->rightChild = insert(tree->rightChild, node);
        }
        else
        {
            return tree;
        }

        tree->height = 1 + max(height(tree->leftChild), height(tree->rightChild));

        int balance = getBalance(tree);
        if (balance > 1)
        {
            if (node->data < tree->leftChild->data)
            {
                tree = rightRotate(tree);
            }
            else
            {
                tree->leftChild = leftRotate(tree->leftChild);
                tree = rightRotate(tree);
            }
        }
        else if (balance < -1)
        {
            if (node->data > tree->rightChild->data)
            {
                tree = leftRotate(tree);
            }
            else
            {
                tree->rightChild = rightRotate(tree->rightChild);
                tree = leftRotate(tree);
            }
        }

        return tree;
    }

    static Node *leftRotate(Node *subtree)
    {
        Node *newRoot = subtree->rightChild;
        subtree->rightChild = newRoot->leftChild;
        newRoot->leftChild = subtree;

        subtree->height = 1 + max(height(subtree->leftChild), height(subtree->rightChild));
        newRoot->height = 1 + max(height(newRoot->leftChild), height(newRoot->rightChild));

        return newRoot;
    }

    static Node *rightRotate(Node *subtree)
    {
        Node *newRoot = subtree->leftChild;
        subtree->leftChild = newRoot->rightChild;
        newRoot->rightChild = subtree;

        subtree->height = 1 + max(height(subtree->leftChild), height(subtree->rightChild));
        newRoot->height = 1 + max(height(newRoot->leftChild), height(newRoot->rightChild));

        return newRoot;
    }

    static Node *newNode(int data)
    {
        return new Node{data, 0, nullptr, nullptr, nullptr};
    }

    static Node *minValueNode(Node *node)
    {
        Node *current = node;
        while (current->leftChild != nullptr)
        {
            current = current->leftChild;
        }
        return current;
    }

    static void order(Node *root, int value)
    {
        if (root != nullptr)
        {
            switch (value)
            {
            case -1:
                cout << root->data << endl;
                order(root->leftChild, value);
                order(root->rightChild, value);
                break;

            case 0:
                order(root->leftChild, value);
                cout << root->data << endl;
                order(root->rightChild, value);
                break;

            case 1:
                order(root->leftChild, value);
                order(root->rightChild, value);
                cout << root->data << endl;
                break;
            }
        }
    }
};

int main()
{
    AVLBinaryTree tree(1);

    tree.add(2);
    tree.add(3);
    tree.add(4);
    tree.add(5);
    tree.add(6);
    tree.add(7);
    tree.add(8);
    tree.add(9);
    AVLBinaryTree::order(tree.root(), 0);

    tree.remove(3);
    cout << endl;
    AVLBinaryTree::order(tree.root(), 0);

    return 0;
}
