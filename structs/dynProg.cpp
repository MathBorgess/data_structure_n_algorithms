#include <iostream>

void fastestWay(int **transportMatrix, int *valueOne, int *valueTwo)
{
    int *fOne = new int[6];
    int *fTwo = new int[6];
    int *pathOne = new int[6];
    int *pathTwo = new int[6];

    fOne[0] = valueOne[0] + transportMatrix[0][0];
    fTwo[0] = valueTwo[0] + transportMatrix[3][0];
    for (int j = 1; j < 6; j++)
    {
        if (fOne[j - 1] <= fTwo[j - 1] + transportMatrix[2][j])
        {
            fOne[j] = fOne[j - 1] + transportMatrix[0][j];
            pathOne[j] = 1;
        }
        else
        {
            fOne[j] = fTwo[j - 1] + transportMatrix[2][j] + transportMatrix[0][j];
            pathOne[j] = 2;
        }
        if (fTwo[j - 1] <= fOne[j - 1] + transportMatrix[1][j])
        {
            fTwo[j] = fTwo[j - 1] + transportMatrix[3][j];
            pathTwo[j] = 2;
        }
        else
        {
            fTwo[j] = fOne[j - 1] + transportMatrix[1][j] + transportMatrix[3][j];
            pathTwo[j] = 1;
        }
    }

    if (fOne[5] + valueOne[1] <= fTwo[5] + valueTwo[1])
    {
        pathOne[5] = 1;
        pathTwo[5] = 1;
    }
    else
    {
        pathOne[5] = 2;
        pathTwo[5] = 2;
    }

    for (int i = 6; i > 0; i--)
    {
        if (pathOne[i] == 1)
        {
            std::cout << "Station " << i << " Line 1" << std::endl;
        }
        else
        {
            std::cout << "Station " << i << " Line 2" << std::endl;
        }
    }

    delete[] fOne;
    delete[] fTwo;
    delete[] pathOne;
    delete[] pathTwo;
};

int main()
{
    int **transportMatrix = new int *[4];
    for (int i = 0; i < 4; i++)
    {
        transportMatrix[i] = new int[6];
    }
    int staticOne[2] = {2, 3};
    int staticTwo[2] = {4, 2};

    transportMatrix[0][0] = 7;
    transportMatrix[0][1] = 9;
    transportMatrix[0][2] = 3;
    transportMatrix[0][3] = 4;
    transportMatrix[0][4] = 8;
    transportMatrix[0][5] = 4;

    transportMatrix[3][0] = 8;
    transportMatrix[3][1] = 5;
    transportMatrix[3][2] = 6;
    transportMatrix[3][3] = 4;
    transportMatrix[3][4] = 5;
    transportMatrix[3][5] = 7;

    transportMatrix[1][1] = 2;
    transportMatrix[1][2] = 3;
    transportMatrix[1][3] = 1;
    transportMatrix[1][4] = 3;
    transportMatrix[1][5] = 4;

    transportMatrix[2][1] = 2;
    transportMatrix[2][2] = 1;
    transportMatrix[2][3] = 2;
    transportMatrix[2][4] = 2;
    transportMatrix[2][5] = 1;

    fastestWay(transportMatrix, staticOne, staticTwo);

    for (int i = 0; i < 4; i++)
    {
        delete[] transportMatrix[i];
    }
    delete[] transportMatrix;

    return 0;
}
