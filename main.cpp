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

    static Node *remove(Node *node)
    {
        if (node->height == 0)
        {
            if (node->antecessor->leftChild == node)
            {
                node->antecessor->leftChild = nullptr;
            }
            else
            {
                node->antecessor->rightChild = nullptr;
            }
            delete node;
        }
        else
        {
            Node *auxNode = sucessor(node);
            swap(node, auxNode);
            // I have to garantee the balance of the tree
            remove(node);
        }
    }

    static Node *swap(Node *node1, Node *node2)
    {
        Node *auxNode = new Node{node1->data, node1->height, node1->antecessor, node1->rightChild, node1->leftChild};
        node1->data = node2->data;
        node1->height = node2->height;
        node2->data = auxNode->data;
        node2->height = auxNode->height;
        delete auxNode;
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
        }
        else if (node->data > tree->data)
        {
            tree->rightChild = insert(tree->rightChild, node);
        }
        else
            return tree;

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
        subtree->rightChild->antecessor = subtree->antecessor;
        subtree->antecessor = subtree->rightChild;
        subtree->rightChild = subtree->rightChild->leftChild;
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
    static Node *critialNode(Node *subtree, int critial)
    {
        if (subtree == nullptr)
        {
            return nullptr;
        }
        else
        {
            if (critial == 1)
            {
                if (subtree->rightChild == nullptr)
                {
                    return subtree;
                }
                else
                {
                    return critialNode(subtree->rightChild, critial);
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
                    return critialNode(subtree->leftChild, critial);
                }
            }
        }
    }

    static Node *sucessor(Node *subtree)
    {
        if (subtree->rightChild != nullptr)
        {
            return subtree->rightChild
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
            if (auxNode != nullptr && auxNode2 nullptr && auxNode != auxNode2->rightChild)
            {
                auxNode = auxNode2;
                auxNode2 = auxNode2->antecessor;
            }
            return auxNode2;
        }
    }

    // order == -1 -> preorder, order == 0 -> inorder, order == 1 -> postorder
    static void order(Node *root, int order)
    {
        if (root != nullptr)
        {
            if (order == -1)
            {
                cout << root->data << endl;
                order(root->leftChild, order);
                order(root->rightChild, order);
            }
            else if (order == 0)
            {
                order(root->leftChild, order);
                cout << root->data << endl;
                order(root->rightChild, order);
            }
            else if (order == 1)
            {
                order(root->leftChild, order);
                order(root->rightChild, order);
                cout << root->data << endl;
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
    AVLBinaryTree::order(tree.root(), -1);
    return 0;
}
