#include <iostream>
#include <cmath>

using namespace std;

struct expressionStruct
{
    float number;
    expressionStruct *next;
    expressionStruct *prev;
};

class ExpressionStack
{
private:
    expressionStruct sentinel = {'+', &sentinel, &sentinel};
    int size_ = 0;

public:
    void push(expressionStruct *node)
    {
        node->next = &sentinel;
        node->prev = sentinel.prev;
        sentinel.prev->next = node;
        sentinel.prev = node;
        size_++;
    }

    expressionStruct *pop()
    {
        sentinel.prev->prev->next = &sentinel;
        expressionStruct *item = sentinel.prev;
        sentinel.prev = sentinel.prev->prev;
        size_--;
        return item;
    }

    void reduce(char *mathOperator)
    {
        float num;
        if (*mathOperator == '+')
        {
            num = sentinel.prev->prev->number + sentinel.prev->number;
        }
        else if (*mathOperator == '-')
        {
            num = sentinel.prev->prev->number - sentinel.prev->number;
        }
        else if (*mathOperator == '*')
        {
            num = sentinel.prev->prev->number * sentinel.prev->number;
        }
        else if (*mathOperator == '/')
        {
            num = sentinel.prev->prev->number / sentinel.prev->number;
        }
        else if (*mathOperator == '^')
        {
            num = pow(sentinel.prev->prev->number, sentinel.prev->number);
        }
        expressionStruct *newNode = new expressionStruct{num, nullptr, nullptr};
        delete pop();
        delete pop();
        push(newNode);
    }

    void search()
    {
        expressionStruct *node = sentinel.next;
        while (node != nullptr && node != &sentinel)
        {
            std::cout << node->number << " ";
            node = node->next;
        }
        std::cout << std::endl;
    }

    float result() const { return sentinel.prev->number; }

    int size() const { return size_; }
};
struct OperatorStruct
{
    char mathOperator;
    OperatorStruct *next;
    OperatorStruct *prev;
};

class OperatorStack
{
private:
    OperatorStruct sentinel = {'+', &sentinel, &sentinel};
    int size_ = 0;

public:
    void push(OperatorStruct *node)
    {
        node->next = &sentinel;
        node->prev = sentinel.prev;
        sentinel.prev->next = node;
        sentinel.prev = node;
        size_++;
    }

    OperatorStruct *pop()
    {
        sentinel.prev->prev->next = &sentinel;
        OperatorStruct *item = sentinel.prev;
        sentinel.prev = sentinel.prev->prev;
        size_--;
        return item;
    }

    void search()
    {
        OperatorStruct *node = sentinel.next;
        while (node != nullptr && node != &sentinel)
        {
            std::cout << node->mathOperator << " ";
            node = node->next;
        }
        std::cout << std::endl;
    }

    int size() const { return size_; }
};

int main()
{
    string inlineExpression;
    OperatorStack mathStack;
    ExpressionStack expStack;
    string postfixExpression = "";
    OperatorStruct *operatorStruct;
    getline(cin, inlineExpression);
    for (char c : inlineExpression)
    {
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
        {
            operatorStruct = new OperatorStruct{c, nullptr, nullptr};
            mathStack.push(operatorStruct);
        }
        else if (c == ')')
        {
            operatorStruct = mathStack.pop();
            expStack.reduce(&operatorStruct->mathOperator);
            postfixExpression += operatorStruct->mathOperator;
        }
        else if (c != ' ' && c != '(')
        {
            postfixExpression += c;
            float num = c - '0';
            expressionStruct *newNode = new expressionStruct{num, nullptr, nullptr};
            expStack.push(newNode);
        }
    }
    if (mathStack.size() > 0)
    {

        operatorStruct = mathStack.pop();
        expStack.reduce(&operatorStruct->mathOperator);
        postfixExpression += operatorStruct->mathOperator;
    }
    cout << postfixExpression << endl;
    cout << expStack.result() << endl;

    return 0;
};
