#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 3
#define Empty 0
#define Max_Operators 4
#define Maxlenght 1000

const char *action[] = {"First State", "Move cell Empty to Up", "Move cell Empty to Down",
                        "Move cell Empty to Left", "Move cell Empty to Right"};

typedef struct
{
    int eightPuzzle[ROWS][COLS];
    int emptyRows;
    int emptyCols;
} State;

void printState(State state)
{
    int row, col;
    printf("\n----------\n");
    for (row = 0; row < ROWS; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            printf("|%d ", state.eightPuzzle[row][col]);
        }
        printf("|\n");
    }
    printf("----------\n");
}

int compareState(State s1, State s2)
{
    if (s1.emptyRows != s2.emptyRows || s1.emptyCols != s2.emptyCols)
        return 0;
    int row, col;
    for (row = 0; row < ROWS; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            if (s1.eightPuzzle[row][col] != s2.eightPuzzle[row][col])
                return 0;
        }
    }
    return 1;
}

int goalCheck(State s, State GOAL)
{
    return compareState(s, GOAL);
}

int upOperators(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRows, emptyColCurrent = state.emptyCols;
    if (emptyRowCurrent > 0)
    {
        result->emptyRows = emptyRowCurrent - 1;
        result->emptyCols = emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent - 1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent - 1][emptyColCurrent] = Empty;
        return 1;
    }
    return 0;
}

int DownOperators(State state, State *result)
{

    *result = state;
    int emptyRowCurrent = state.emptyRows, emptyColCurrent = state.emptyCols;
    if (emptyRowCurrent < 2)
    {
        result->emptyRows = emptyRowCurrent + 1;
        result->emptyCols = emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent + 1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent + 1][emptyColCurrent] = Empty;
        return 1;
    }
    return 0;
}

int leftOperators(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRows, emptyColCurrent = state.emptyCols;
    if (emptyColCurrent > 0)
    {
        result->emptyRows = emptyRowCurrent;
        result->emptyCols = emptyColCurrent - 1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent][emptyColCurrent - 1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent - 1] = Empty;
        return 1;
    }
    return 0;
}

int rightOperators(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRows, emptyColCurrent = state.emptyCols;
    if (emptyColCurrent < 2)
    {
        result->emptyRows = emptyRowCurrent;
        result->emptyCols = emptyColCurrent + 1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent][emptyColCurrent + 1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent + 1] = Empty;
        return 1;
    }
    return 0;
}

// dem so o sai khac voi o muc tieu
int heuristic1(State state, State goal)
{
    int row, col, count = 0;
    for (row = 0; row < ROWS; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            if (state.eightPuzzle[row][col] != goal.eightPuzzle[row][col])
                count++;
        }
    }

    return count;
}

int heuristic2(State state, State goal)
{
    int count = 0;
    int row, col, row_g, col_g;
    for (row = 0; row < ROWS; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            if (state.eightPuzzle[row][col] != Empty)
            {
                for (row_g = 0; row_g < ROWS; row_g++)
                {
                    for (col_g = 0; col_g < COLS; col_g++)
                    {
                        if (state.eightPuzzle[row][col] == goal.eightPuzzle[row_g][col_g])
                        {
                            count += abs(row - row_g) + abs(col - col_g);
                            col_g = COLS;
                            row_g = ROWS;
                        }
                    }
                }
            }
        }
    }
    return count;
}

typedef struct Node
{
    State state;
    struct Node *parent;
    int no_function;
    int f, g, h;
} Node;

typedef struct
{
    Node *Element[Maxlenght];
    int size;
} List;

void makeNullList(List *pL)
{
    pL->size = 0;
}

int emptyList(List L)
{
    return L.size == 0;
}

int full_List(List L)
{
    return L.size == Maxlenght;
}

Node *element_at(int p, List list)
{
    return list.Element[p - 1];
}

void push_List(Node *x, int position, List *list)
{
    if (!full_List(*list))
    {
        int q;
        for (q = list->size; q >= position; q--)
        {
            list->Element[q] = list->Element[q - 1];
        }
        list->Element[position - 1] = x;
        list->size++;
    }
    else
        printf("List is full!\n");
}

void delete_List(int position, List *list)
{
    if (emptyList(*list))
        printf("List is empty!\n");
    else if (position < 1 || position > list->size)
        printf("Position is not possible to delete!\n");
    else
    {
        int i;
        for (i = position - 1; i < list->size - 1; i++)
        {
            list->Element[i] = list->Element[i + 1];
        }
        list->size--;
    }
}

Node *findState(State state, List list, int *position)
{
    int i;
    for (i = 1; i <= list.size; i++)
    {
        if (compareState(element_at(i, list)->state, state))
        {
            *position = i;
            return element_at(i, list);
        }
    }
    return NULL;
}

void sortList(List *list)
{
    int i, j;
    for (i = 0; i < list->size - 1; i++)
    {
        for (j = i + 1; j < list->size; j++)
        {
            if (list->Element[i]->h > list->Element[j]->h)
            {
                Node *node = list->Element[i];
                list->Element[i] = list->Element[j];
                list->Element[j] = node;
            }
        }
    }
}

int call_operators(State state, State *result, int opt)
{
    switch (opt)
    {
    case 1:
        return upOperators(state, result);
    case 2:
        return DownOperators(state, result);
    case 3:
        return leftOperators(state, result);
    case 4:
        return rightOperators(state, result);
    default:
        printf("Cannot call operators");
        return 0;
    }
}

Node *A_Star(State state, State goal)
{
    List Open_A_Star;
    List Close_A_Star;
    makeNullList(&Open_A_Star);
    makeNullList(&Close_A_Star);

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    root->g = 0;
    root->h = heuristic1(root->state, goal);
    root->f = root->g + root->h;
    push_List(root, Open_A_Star.size + 1, &Open_A_Star);
    while (!emptyList(Open_A_Star))
    {
        Node *node = element_at(1, Open_A_Star);
        delete_List(1, &Open_A_Star);
        push_List(node, Close_A_Star.size + 1, &Close_A_Star);
        if (goalCheck(node->state, goal))
        {
            return node;
        }
        int opt;
        for (opt = 1; opt <= Max_Operators; opt++)
        {
            State newState;
            if (call_operators(node->state, &newState, opt))
            {
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->no_function = opt;
                newNode->g = node->g+1;
                newNode->h = heuristic1(newState, goal);
                newNode->f = newNode->g + newNode->h;
                int pos_Open, pos_Close;
                Node *nodeFoundOpen = findState(newState, Open_A_Star, &pos_Open);
                Node *nodeFoundClose = findState(newState, Close_A_Star, &pos_Close);

                if (nodeFoundOpen == NULL && nodeFoundClose == NULL)
                    push_List(newNode, Open_A_Star.size + 1, &Open_A_Star);
                else if (nodeFoundOpen != NULL && nodeFoundOpen->g > newNode->g)
                {
                    delete_List(pos_Open, &Open_A_Star);
                    push_List(newNode, pos_Open, &Open_A_Star);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->g > newNode->g)
                {
                    delete_List(pos_Close, &Close_A_Star);
                    push_List(newNode, Open_A_Star.size+1, &Open_A_Star);
                }
            }
        }
        sortList(&Open_A_Star);
    }
    return NULL;
}

void print_WaytogetGoal(Node *node)
{
    List listPrint;
    makeNullList(&listPrint);

    while (node->parent)
    {
        push_List(node, listPrint.size + 1, &listPrint);
        node = node->parent;
    }
    push_List(node, listPrint.size + 1, &listPrint);

    int no_action = 0, i;
    for (i = listPrint.size; i > 0; i--)
    {
        printf("\nAction %d: %s", no_action, action[element_at(i, listPrint)->no_function]);
        printState(element_at(i, listPrint)->state);
        no_action++;
    }
}

int main()
{
    State state;
    state.emptyRows = 1;
    state.emptyCols = 1;
    state.eightPuzzle[0][0] = 1;
    state.eightPuzzle[0][1] = 2;
    state.eightPuzzle[0][2] = 3;
    state.eightPuzzle[1][0] = 8;
    state.eightPuzzle[1][1] = 0;
    state.eightPuzzle[1][2] = 4;
    state.eightPuzzle[2][0] = 7;
    state.eightPuzzle[2][1] = 6;
    state.eightPuzzle[2][2] = 5;

    State goal;
    goal.emptyRows = 1;
    goal.emptyCols = 0;
    goal.eightPuzzle[0][0] = 2;
    goal.eightPuzzle[0][1] = 8;
    goal.eightPuzzle[0][2] = 1;
    goal.eightPuzzle[1][0] = 0;
    goal.eightPuzzle[1][1] = 4;
    goal.eightPuzzle[1][2] = 3;
    goal.eightPuzzle[2][0] = 7;
    goal.eightPuzzle[2][1] = 6;
    goal.eightPuzzle[2][2] = 5;

    Node *p = A_Star(state, goal);
    print_WaytogetGoal(p);
    return 0;
}