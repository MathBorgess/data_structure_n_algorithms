#include <iostream>
#include <chrono>

using namespace std;

struct Node
{
    int data;
    int height;
    Node *parent;
    Node *rightChild;
    Node *leftChild;
};

class AVLBinaryTree
{
private:
    Node *root_;

public:
    AVLBinaryTree() : root_(nullptr) {}
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

    Node *remove(int data)
    {
        Node *node = find(data);
        if (node != nullptr)
        {
            root_ = delNode(root_, node);
            return root_
        }
        return nullptr;
    }

    void clear()
    {
        clear_(root_);
        root_ = nullptr;
    }

    void order(int value)
    {
        order(root_, value);
    }

    static void clear_(Node *subtree)
    {
        if (subtree != nullptr)
        {
            clear_(subtree->leftChild);
            clear_(subtree->rightChild);
            delete subtree;
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
                if (temp != nullptr)
                {
                    temp->parent = tree->parent;
                }
                delete tree;
                return temp;
            }
            else if (tree->rightChild == nullptr)
            {
                Node *temp = tree->leftChild;
                if (temp != nullptr)
                {
                    temp->parent = tree->parent;
                }
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
        node->parent = tree;

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
        newRoot->parent = subtree->parent;
        subtree->rightChild = newRoot->leftChild;
        if (newRoot->leftChild != nullptr)
            newRoot->leftChild->parent = subtree;
        newRoot->leftChild = subtree;
        subtree->parent = newRoot;

        subtree->height = 1 + max(height(subtree->leftChild), height(subtree->rightChild));
        newRoot->height = 1 + max(height(newRoot->leftChild), height(newRoot->rightChild));

        return newRoot;
    }

    static Node *rightRotate(Node *subtree)
    {
        Node *newRoot = subtree->leftChild;
        newRoot->parent = subtree->parent;
        subtree->leftChild = newRoot->rightChild;
        if (newRoot->rightChild != nullptr)
            newRoot->rightChild->parent = subtree;
        newRoot->rightChild = subtree;
        subtree->parent = newRoot;

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

    static int nodeLevel(Node *tree, int count = 0)
    {
        if (tree->parent != nullptr)
        {
            count++;
            count = nodeLevel(tree->parent, count);
        }
        return count;
    }
};

int main()
{
    AVLBinaryTree *tree = new AVLBinaryTree(1);
    tree->add(2);
    cout << tree->find(3)->data << endl;
    // tree->add(3);
    // tree->add(4);
    // tree->add(5);
    // tree->add(6);
    // tree->add(7);
    // tree->add(8);
    // tree->add(9);

    // tree->remove(3);
    // // stressTest
    // tree->clear();
    // delete tree;

    // auto start_time = chrono::high_resolution_clock::now();

    // tree = new AVLBinaryTree(1);
    // for (int i = 0; i < 10000000; i++)
    // {
    //     tree->add(i);
    // }

    // auto end_time = chrono::high_resolution_clock::now();

    // auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    // cout << "Execution time: " << duration_ms << " milliseconds" << endl;

    // start_time = chrono::high_resolution_clock::now();
    // for (int i = 0; i < 10000000; i++)
    // {
    //     tree->remove(i);
    // }

    // end_time = chrono::high_resolution_clock::now();

    // duration_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    // cout << "Execution time: " << duration_ms << " milliseconds" << endl;

    return 0;
}
