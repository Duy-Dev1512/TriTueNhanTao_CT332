#include <stdio.h>

int matrix[8][8];

int isPossible(int n, int row, int col)
{
    for (int i = row - 1; i >= 0; i--)
    {
        if (matrix[i][col] == 1)
            return 0;
    }

    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (matrix[i][j] == 1)
            return 0;
    }


    for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++)
    {
        if (matrix[i][j] == 1)
            return 0;
    }
    return 1;
}

void QueenHelper(int n, int row)
{
    if (row == n)
    {
        printf("_________________________________\n");
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++){
                if(matrix[i][j]==1){
                    printf("|\033[0;32m %d ", matrix[i][j]);
                }
                else printf("|\033[0;30m - ");
                
            }
                
            printf("|\n");
        }
        return;
    }

    for (int j = 0; j < n; j++)
    {
        if (isPossible(n, row, j))
        {
            matrix[row][j] = 1;
            QueenHelper(n, row + 1);
        }
        matrix[row][j] = 0;
    }
    return;
}

void placeQueen(int n)
{

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = 0;
        }
    }
    QueenHelper(n, 0);
}

int main()
{
    int n = 8;
    placeQueen(n);
    return 0;
}