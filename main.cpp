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
            delNode(node);
        }
    }

    static void *delNode(Node *node)
    {
        Node *auxNode;
        if (node->height <= 1)
        {
            if (node->antecessor->leftChild == node)
            {
                node->antecessor->leftChild = nullptr;
            }
            else
            {
                node->antecessor->rightChild = nullptr;
            }
            AVLBinaryTree::order(node->antecessor, -1);
            node->antecessor->height = max(height(node->antecessor->leftChild), height(node->antecessor->rightChild)) + 1;
            int balance = getBalance(node->antecessor);
            int childBalance;
            if (balance > 1)
            {
                childBalance = getBalance(node->antecessor->leftChild);
                if (childBalance < -1)
                {
                    node->antecessor->leftChild = leftRotate(node->antecessor->leftChild);
                }
                node->antecessor = rightRotate(node->antecessor);
            }
            else if (balance < -1)
            {
                childBalance = getBalance(node->antecessor->rightChild);
                if (childBalance > 1)
                {
                    node->antecessor->rightChild = rightRotate(node->antecessor->rightChild);
                }
                node->antecessor = leftRotate(node->antecessor);
            }
            delete node;
        }
        else
        {
            auxNode = sucessor(node);
            int auxData = node->data;
            node->data = auxNode->data;
            auxNode->data = auxData;
            delNode(auxNode);
        }
    }

    static int height(Node *node)
    {
        if (node == nullptr)
        {
            return 0; // Assuming leaf nodes have height 0
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
            tree->leftChild->antecessor = tree;
        }
        else if (node->data > tree->data)
        {
            tree->rightChild = insert(tree->rightChild, node);
            tree->rightChild->antecessor = tree;
        }
        else
            return tree;
        tree->height = 1 + max(height(tree->leftChild), height(tree->rightChild));

        int balance = getBalance(tree);

        if (balance > 1)
        {
            if (node->data >= tree->leftChild->data)
            {
                tree->leftChild = leftRotate(tree->leftChild);
            }
            tree = rightRotate(tree);
        }
        else if (balance < -1)
        {
            if (node->data <= tree->rightChild->data)
            {
                tree->rightChild = rightRotate(tree->rightChild);
            }
            tree = leftRotate(tree);
        }

        return tree;
    }

    static Node *leftRotate(Node *subtree)
    {
        subtree->rightChild->antecessor = subtree->antecessor;
        subtree->antecessor = subtree->rightChild;
        subtree->rightChild = subtree->rightChild->leftChild;
        if (subtree->antecessor->leftChild)
            subtree->antecessor->leftChild->antecessor = subtree;
        subtree->antecessor->leftChild = subtree;
        subtree = subtree->antecessor;
        subtree->height = max(height(subtree->leftChild), height(subtree->rightChild)) + 1;
        subtree->leftChild->height = max(height(subtree->leftChild->leftChild), height(subtree->leftChild->rightChild)) + 1;
        return subtree;
    }

    static Node *rightRotate(Node *subtree)
    {
        subtree->leftChild->antecessor = subtree->antecessor;
        subtree->antecessor = subtree->leftChild;
        subtree->leftChild = subtree->leftChild->rightChild;
        if (subtree->antecessor->rightChild)
            subtree->antecessor->rightChild->antecessor = subtree;
        subtree->antecessor->rightChild = subtree;
        subtree = subtree->antecessor;
        subtree->height = max(height(subtree->leftChild), height(subtree->rightChild)) + 1;
        subtree->rightChild->height = max(height(subtree->rightChild->leftChild), height(subtree->rightChild->rightChild)) + 1;

        return subtree;
    }

    static Node *newNode(int data)
    {
        return new Node{data, 0, nullptr, nullptr, nullptr};
    }

    // critical == 1 -> max node, critical == -1 -> min
    static Node *critialNode(Node *subtree, int critical)
    {
        if (subtree == nullptr)
        {
            return nullptr;
        }
        else
        {
            if (critical == 1)
            {
                if (subtree->rightChild == nullptr)
                {
                    return subtree;
                }
                else
                {
                    return critialNode(subtree->rightChild, critical);
                }
            }
            else if (critical == -1)
            {
                if (subtree->leftChild == nullptr)
                {
                    return subtree;
                }
                else
                {
                    return critialNode(subtree->leftChild, critical);
                }
            }
        }
    }

    static Node *sucessor(Node *subtree)
    {
        if (subtree->rightChild != nullptr)
        {
            return subtree->rightChild;
        }
        else
        {
            Node *nodeFather = subtree->antecessor;
            if (nodeFather == nullptr)
            {
                return nullptr;
            }
            Node *nodeGrandpa = nodeFather->antecessor;
            while (nodeFather != nullptr && nodeGrandpa != nullptr && nodeFather != nodeGrandpa->leftChild)
            {
                nodeFather = nodeGrandpa;
                nodeGrandpa = nodeGrandpa->antecessor;
            }
            return nodeGrandpa;
        }
    }

    static Node *antecessor(Node *subtree)
    {
        Node *auxNode;
        if (subtree->leftChild != nullptr)
        {
            auxNode = subtree->leftChild;
            while (auxNode != nullptr && auxNode->rightChild != nullptr)
            {
                auxNode = auxNode->rightChild;
            }
            return auxNode;
        }
        else
        {
            auxNode = subtree;
            Node *auxNode2 = subtree->antecessor;
            if (auxNode != nullptr && auxNode2 != nullptr && auxNode != auxNode2->rightChild)
            {
                auxNode = auxNode2;
                auxNode2 = auxNode2->antecessor;
            }
            return auxNode2;
        }
    }

    // value == -1 -> preorder, value == 0 -> inorder, value == 1 -> postorder
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
    tree.remove(1);
    AVLBinaryTree::order(tree.root(), 0);
    return 0;
}
