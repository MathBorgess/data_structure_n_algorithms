#include <iostream>

using namespace std;

int main()
{
    int bag_weight = 10;
    int n = 3;
    int *wegihts = new int[n];

    for (int i = 0; i < n; i++)
    {
        cin >> wegihts[i];
    }
    int **dp = new int *[n];
    for (int i = 0; i < n; i++)
    {
        dp[i] = new int[bag_weight + 1];
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j <= bag_weight; j++)
        {
            if (j >= wegihts[i - 1])
            {
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - wegihts[i - 1]] + wegihts[i - 1]);
            }
            else
            {
                dp[i - 1][j] = dp[i - 2][j];
            }
        }
    }

    delete[] wegihts;
    for (int i = 0; i < n; i++)
    {
        delete[] dp[i];
    }
    delete[] dp;
    return 0;
}