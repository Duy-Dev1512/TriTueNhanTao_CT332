#include <stdio.h>
#include <stdlib.h>

#define Max_Value 10
#define Empty 0
#define AREA_SQUARE_SIZE 3
#define INF 999999
#define MAX_Lenght 1000
#define NB_Rows 9
#define NB_Columns 9

typedef struct
{
    int x, y;
} Coord;

typedef struct
{
    Coord data[MAX_Lenght];
    int size;
} ListCoord;

typedef struct
{
    int data[NB_Rows * NB_Columns][NB_Rows * NB_Columns];
    int n;
} Constrains;

void initListCoord(ListCoord *list)
{
    list->size = 0;
}

void appendListCoord(ListCoord *list, Coord coord)
{
    list->data[list->size++] = coord;
}

void initConstrains(Constrains *cons)
{
    int i, j;
    for (i = 0; i < NB_Rows * NB_Columns; i++)
    {
        for (j = 0; j < NB_Rows * NB_Columns; j++)
        {
            cons->data[i][j] = 0;
        }
    }
    cons->n = NB_Rows * NB_Columns;
}

int indexOf(Coord coo)
{
    return (NB_Rows * coo.x + coo.y);
}

Coord positionOfVertex(int vertex)
{
    Coord coord;
    coord.x = vertex / NB_Rows;
    coord.y = vertex % NB_Columns;
    return coord;
}

int addConstrains(Constrains *cons, Coord source, Coord target)
{
    int u = indexOf(source);
    int v = indexOf(target);
    if (cons->data[u][v] == 0)
    {
        cons->data[u][v] = 1;
        cons->data[v][u] = 1;
        return 1;
    }
    return 0;
}

ListCoord getConstrains(Constrains cons, Coord coo)
{
    int v = indexOf(coo);
    ListCoord result;
    initListCoord(&result);
    int i;
    for (i = 0; i < cons.n; i++)
    {
        if (cons.data[v][i] == 1)
        {
            appendListCoord(&result, positionOfVertex(i));
        }
    }
    return result;
}

typedef struct
{
    int cell[NB_Rows][NB_Columns];
    Constrains constrains;
} Sudoku;

void initSodoku(Sudoku *sudoku)
{
    int i, j;
    for (i = 0; i < NB_Rows; i++)
    {
        for (j = 0; j < NB_Columns; j++)
        {
            sudoku->cell[i][j] = Empty;
        }
    }
    initConstrains(&sudoku->constrains);
}

void initSodokuWithValues(Sudoku *sudoku, int inputs[NB_Rows][NB_Columns])
{
    int i, j;
    for (i = 0; i < NB_Rows; i++)
    {
        for (j = 0; j < NB_Columns; j++)
        {
            sudoku->cell[i][j] = inputs[i][j];
        }
    }
    initConstrains(&sudoku->constrains);
}

void printSudoku(Sudoku sudoku)
{
    int i, j;
    printf("Sudoku: \n");
    for (i = 0; i < NB_Rows; i++)
    {
        if (i % AREA_SQUARE_SIZE == 0)
            printf("-------------------------\n");
        for (j = 0; j < NB_Columns; j++)
        {
            if (j % AREA_SQUARE_SIZE == 0)
                printf("| ");
            printf("%d ", sudoku.cell[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------\n");
}

int isFilledSudoku(Sudoku sudoku)
{
    int i, j;
    for (i = 0; i < NB_Rows; i++)
    {
        for (j = 0; j < NB_Columns; j++)
        {
            if (sudoku.cell[i][j] == Empty)
                return 0;
        }
    }
    return 1;
}

void spreadConstrainsFrom(Coord position, Constrains *constrains, ListCoord *changed)
{
    int row = position.x, column = position.y;
    int i, j;

    // create contraints col
    for (i = 0; i < NB_Rows; i++)
    {
        if (i != row)
        {
            Coord pos_r = {i, column};
            if (addConstrains(constrains, position, pos_r))
                appendListCoord(changed, pos_r);
        }
    }
    // create contraints row
    for (i = 0; i < NB_Columns; i++)
    {
        if (i != column)
        {
            Coord pos_c = {row, i};
            if (addConstrains(constrains, position, pos_c))
                appendListCoord(changed, pos_c);
        }
    }

    // contraints block
    for (i = 0; i < AREA_SQUARE_SIZE; i++)
    {
        for (j = 0; j < AREA_SQUARE_SIZE; j++)
        {
            int areaX = (row / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
            int areaY = (column / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
            if (areaX + i != row || areaY + j != column)
            {
                Coord pos_block = {areaX + i, areaY + j};
                if (addConstrains(constrains, position, pos_block))
                    appendListCoord(changed, pos_block);
            }
        }
    }
}

typedef int ElementType;
typedef struct
{
    ElementType Element[MAX_Lenght];
    int size;
} List;

void initList(List *list)
{
    list->size = 0;
}

void appendList(List *list, ElementType x)
{
    list->Element[list->size++] = x;
}

List getAvailableValues(Coord position, Sudoku sudoku)
{
    ListCoord posList = getConstrains(sudoku.constrains, position);
    int available[Max_Value];
    int i;
    for (i = 1; i < Max_Value; i++)
        available[i] = 1;
    for (i = 0; i < posList.size; i++)
    {
        Coord pos = posList.data[i];
        if (sudoku.cell[pos.x][pos.y] != Empty)
        {
            available[sudoku.cell[pos.x][pos.y]] = 0;
        }
    }
    List result;
    initList(&result);
    for (i = 1; i < Max_Value; i++)
    {
        if (available[i])
            appendList(&result, i);
    }
    return result;
}

Coord getNextEmptyCell(Sudoku sudoku)
{
    int i, j;
    for (i = 0; i < NB_Rows; i++)
    {
        for (j = 0; j < NB_Columns; j++)
        {
            Coord pos = {i, j};
            if (sudoku.cell[i][j] == Empty)
                return pos;
        }
    }
}

Coord getNextMinDomainCell(Sudoku sudoku)
{
    int minLength = INF;
    int i, j;
    Coord result;
    for (i = 0; i < NB_Rows; i++)
    {
        for (j = 0; j < NB_Columns; j++)
        {
            if (sudoku.cell[i][j] == Empty)
            {
                Coord pos = {i, j};
                int availablesLength = getAvailableValues(pos, sudoku).size;
                if (availablesLength < minLength)
                {
                    minLength = availablesLength;
                    result = pos;
                }
            }
        }
    }
    return result;
}

int exploredCounter = 0;
int sudokuBackStracking(Sudoku *sudoku)
{
    if (isFilledSudoku(*sudoku))
        return 1;

    Coord position = getNextMinDomainCell(*sudoku);
    List availables = getAvailableValues(position, *sudoku);
    if (availables.size == 0)
        return 0;

    int j;
    for (j = 0; j < availables.size; j++)
    {
        int value = availables.Element[j];
        sudoku->cell[position.x][position.y] = value;
        exploredCounter++;
//        ListCoord history;
//        initListCoord(&history);
        if (sudokuBackStracking(sudoku))
            return 1;
        sudoku->cell[position.x][position.y] = Empty;
    }
    return 0;
}

Sudoku solveSudoku(Sudoku sudoku)
{
    int i, j;
    for (i = 0; i < NB_Rows; i++)
    {
        for (j = 0; j < NB_Columns; j++)
        {
            if (sudoku.cell[i][j] == Empty)
            {
                ListCoord history;
                initListCoord(&history);
                Coord pos = {i, j};
                spreadConstrainsFrom(pos, &sudoku.constrains, &history);
            }
        }
    }
    exploredCounter = 0;
    if (sudokuBackStracking(&sudoku))
        printf("Solved\n");
    else
        printf("Can not solved\n");
    printf("Explored %d states\n", exploredCounter);
    return sudoku;
}

int inputs1[9][9] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9},
};

int inputs2[9][9] = {
	{0, 6, 1, 0, 0, 7, 0, 0, 3},
    {0, 9, 2, 0, 0, 3, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 8, 5, 3, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 5, 0, 4},
    {5, 0, 0, 0, 0, 8, 0, 0, 0},
    {0, 4, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 1, 6, 0, 8, 0, 0},
    {6, 0, 0, 0, 0, 0, 0, 0, 0},
};

int main()
{
    Sudoku sudoku;
    initSodokuWithValues(&sudoku, inputs2);
    printSudoku(sudoku);
    Sudoku result = solveSudoku(sudoku);
    printSudoku(result);
    return 0;
}
