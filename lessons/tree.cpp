#include <iostream>

using namespace std;

struct ListNode
{
    int data;
    ListNode *next;
    ListNode *prev;
};

class LinkedList
{
private:
    ListNode sentinel = {0, &sentinel, &sentinel};

public:
    void push(ListNode *node)
    {
        node->next = &sentinel;
        node->prev = sentinel.prev;
        sentinel.prev->next = node;
        sentinel.prev = node;
    }

    void search()
    {
        ListNode *node = sentinel.next;
        cout << "[";
        if (node != nullptr && node != &sentinel)
        {
            cout << node->data;
            node = node->next;
        }

        while (node != nullptr && node != &sentinel)
        {
            cout << "," << node->data;
            node = node->next;
        }
        cout << "]" << endl;
    }
};

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
        while (auxNode != nullptr)
        {
            if (auxNode->data == data)
            {
                return auxNode;
            }
            else if (data < auxNode->data)
            {
                auxNode = auxNode->leftChild;
            }
            else
            {
                auxNode = auxNode->rightChild;
            }
        }
        return nullptr;
    }

    int findLevel(int data)
    {
        Node *auxNode = root_;
        int count = 0;
        while (auxNode != nullptr)
        {
            if (auxNode->data == data)
            {
                return count;
            }
            else if (data < auxNode->data)
            {
                count++;
                auxNode = auxNode->leftChild;
            }
            else
            {
                count++;
                auxNode = auxNode->rightChild;
            }
        }
        return -1;
    }

    Node *remove(int data)
    {
        Node *node = find(data);
        if (node != nullptr)
        {
            root_ = delNode(root_, node);
            return node;
        }
        return nullptr;
    }

    void order(int value)
    {
        LinkedList *list = new LinkedList();
        order(root_, value, list);
        list->search();
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

    static void order(Node *root, int value, LinkedList *result)
    {
        if (root != nullptr)
        {
            switch (value)
            {
            case -1:
                result->push(new ListNode{root->data, nullptr, nullptr});
                order(root->leftChild, value, result);
                order(root->rightChild, value, result);
                break;

            case 0:
                order(root->leftChild, value, result);
                result->push(new ListNode{root->data, nullptr, nullptr});
                order(root->rightChild, value, result);
                break;

            case 1:
                order(root->leftChild, value, result);
                order(root->rightChild, value, result);
                result->push(new ListNode{root->data, nullptr, nullptr});
                break;
            }
        }
    }
};

int main()
{
    string command;
    int value;
    Node *node;
    AVLBinaryTree *tree = new AVLBinaryTree();
    while (command != "FIM")
    {
        cin >> command;
        if (command == "ADICIONA")
        {
            cin >> value;
            tree->add(value);
        }
        else if (command == "REMOVE")
        {
            cin >> value;
            node = tree->remove(value);

            if (node == nullptr)
            {
                cout << "Valor " << value << " inexistente" << endl;
            }
        }
        else if (command == "NIVEL")
        {
            cin >> value;
            int nodeLevel = tree->findLevel(value);
            if (nodeLevel != -1)
            {
                cout << "Nivel de " << value << ": " << nodeLevel << endl;
            }
            else
            {
                cout << "Valor " << value << " inexistente" << endl;
            }
        }
        else if (command == "PRINT")
        {
            cin >> command;

            if (command == "PREORDEM")
            {
                tree->order(-1);
            }
            else if (command == "EMORDEM")
            {
                tree->order(0);
            }
            else if (command == "POSORDEM")
            {
                tree->order(1);
            };
        }
    };
    return 0;
}