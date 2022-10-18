#include <iostream>
#include <stdlib.h>

#include <vector>
#include <algorithm>
// #include<iterator>

using namespace std;

#define ROWS 3
#define COLS 3
#define Empty 0
#define Max_Operation 4
#define Maxlenght 50000

const char *action[] = {"First State", "Move cell Empty to Up", "Move cell Empty to Down",
                        "Move cell Empty to Left", "Move cell Empty to Right"};

typedef struct
{
    int eightPuzzle[ROWS][COLS];
    int emptyRows;
    int emptyCols;
} State;

typedef struct Node
{
    State state;
    struct Node *parent;
    int no_function;
    int heuristic;
} Node;
void printState(State state)
{
    int row, col;
    cout<<"\n----------\n";
    for (row = 0; row < ROWS; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            cout<<"| "<< state.eightPuzzle[row][col]<<" ";
        }
        cout<<"|\n";
    }
    cout<<"----------\n";
}

int compareState(State s1, State s2)
{
    if (s1.emptyRows != s2.emptyRows || s1.emptyCols != s2.emptyCols)
        return 0;
    int row, col;
    for (row = 0; row < ROWS; row++)
        for (col = 0; col < COLS; col++)
            if (s1.eightPuzzle[row][col] != s2.eightPuzzle[row][col])
                return 0;
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
        cout<<"Cannot call operators";
        return 0;
    }
}

int heuristic1(State state, State goal)
{
    int row, col, count = 0;
    for (row = 0; row < ROWS; row++)
        for (col = 0; col < COLS; col++)
            if (state.eightPuzzle[row][col] != goal.eightPuzzle[row][col])
                count++;

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

Node *findState(State state, vector<Node *> v, vector<Node *>::iterator *position)
{
    vector<Node *>::iterator it = v.begin();
    if (v.size() == 0)
        return NULL;

    while (it != v.end())
    {
        if (compareState((*it)->state, state))
        {
            *position = it;
            return *it;
        }
        it = v.erase(it);
    }
    return NULL;
}

bool compareHeuristic(Node *a, Node *b)
{
    return a->heuristic > b->heuristic;
}

Node *best_first_search(State state, State goal)
{
    vector<Node *> OpenBFS(Maxlenght);
    OpenBFS.clear();
    vector<Node *> CloseBFS(Maxlenght);
    CloseBFS.clear();

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    root->heuristic = heuristic1(root->state, goal);
    OpenBFS.push_back(root);
    while (!OpenBFS.empty())
    {
        Node *node = OpenBFS.back();
        OpenBFS.pop_back();
        CloseBFS.push_back(node);
        if (goalCheck(node->state, goal))
        {
            cout << "Goal\n";
            return node;
        }
        int opt;
        for (opt = 1; opt <= Max_Operation; opt++)
        {
            State newState;
            if (call_operators(node->state, &newState, opt))
            {
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->no_function = opt;
                newNode->heuristic = heuristic1(newState, goal);

                vector<Node *>::iterator pos_Open, pos_Close;
                Node *nodeFoundOpen = findState(newState, OpenBFS, &pos_Open);
                Node *nodeFoundClose = findState(newState, CloseBFS, &pos_Close);
                if (nodeFoundOpen == NULL && nodeFoundClose == NULL)
                {
                    OpenBFS.push_back(newNode);
                }
                else if (nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic)
                {
                    OpenBFS.erase(pos_Open);
                    OpenBFS.push_back(newNode);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic)
                {
                    CloseBFS.erase(pos_Close);
                    OpenBFS.push_back(newNode);
                }
            }
        }
        sort(OpenBFS.begin(), OpenBFS.end(), compareHeuristic);
    }
    return NULL;
}

void print_WaysToGetGoal(Node *node)
{
    vector<Node *> vectorPrint;

    while (node->parent != NULL)
    {
        vectorPrint.push_back(node);
        node = node->parent;
    }
    vectorPrint.push_back(node);

    int no_action = 0, i;
    for (i = vectorPrint.size() - 1; i >= 0; i--)
    {
        cout << "\nAction "<< no_action <<": " << action[vectorPrint.at(i)->no_function];
        printState(vectorPrint.at(i)->state);
        no_action++;
    }
}
int main()
{
    State state;
    state.emptyRows = 1;
    state.emptyCols = 1;
    state.eightPuzzle[0][0] = 3;
    state.eightPuzzle[0][1] = 4;
    state.eightPuzzle[0][2] = 5;
    state.eightPuzzle[1][0] = 1;
    state.eightPuzzle[1][1] = 0;
    state.eightPuzzle[1][2] = 2;
    state.eightPuzzle[2][0] = 6;
    state.eightPuzzle[2][1] = 7;
    state.eightPuzzle[2][2] = 8;

    State goal;
    goal.emptyRows = 0;
    goal.emptyCols = 0;
    goal.eightPuzzle[0][0] = 0;
    goal.eightPuzzle[0][1] = 1;
    goal.eightPuzzle[0][2] = 2;
    goal.eightPuzzle[1][0] = 3;
    goal.eightPuzzle[1][1] = 4;
    goal.eightPuzzle[1][2] = 5;
    goal.eightPuzzle[2][0] = 6;
    goal.eightPuzzle[2][1] = 7;
    goal.eightPuzzle[2][2] = 8;
    Node *p = best_first_search(state, goal);
    print_WaysToGetGoal(p);
    return 0;
}
