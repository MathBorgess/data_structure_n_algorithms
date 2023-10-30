#include <iostream>
#include <string>

using namespace std;

int main()
{
    int count = 0;
    string curlyBrackets;
    getline(cin, curlyBrackets);

    for (auto character : curlyBrackets)
    {
        if (character == '{')
        {
            count++;
        }
        else if (character == '}')
        {
            count--;
        }
        if (count < 0)
        {
            cout << 'N' << endl;
            return 0;
        }
    }
    if (count == 0)
    {
        cout << 'S' << endl;
    }
    else
    {
        cout << 'N' << endl;
    }
    return 0;
}
