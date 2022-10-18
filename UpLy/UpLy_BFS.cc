#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>

using namespace std;

#define MaxLength 100
#define Max_Cup 6

typedef struct
{
    int A[Max_Cup];
} State;

void makeNullState(State *state)
{
    int i;
    for (i = 0; i < Max_Cup; i++)
    {
        if (i % 2 == 0)
            state->A[i] = 1;
        else
            state->A[i] = -1;
    }
}

typedef struct Node
{
    State state;
    struct Node *parent;
    int no_function;
} Node;

int compareStates(State *state1, State *state2)
{
    int i;
    for (i = 0; i < 6; i++)
    {
        if (state1->A[i] != state2->A[i])
            return 0;
    }
    return 1;
}

int findState(State state, queue<Node *> openQueue)
{
    while (!openQueue.empty())
    {
        if (compareStates(&openQueue.front()->state, &state))
        {
            return 1;
        }
        openQueue.pop();
    }
    return 0;
}

int goal_check(State s)
{
    int i;
    for (i = 0; i < 6; i++)
    {
        if (s.A[i] != 1)
            return 0;
    }
    return 1;
}

void xoay3ly(State current, State *result, int x)
{
    int i;

    for (i = 0; i < 6; i++)
    {
        result->A[i] = current.A[i];
    }
    for (i = x; i <= x + 2; i++)
    {
        result->A[i] *= -1;
    }
}

void print_state(State s)
{
    int i;
    printf("State: ");
    for (i = 0; i < 6; i++)
    {
        printf("%d ", s.A[i]);
    }
}

Node *BFS_Algorithm(State state)
{
    queue<Node *> Open_BFS;
    queue<Node *> Close_BFS;

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    Open_BFS.push(root);

    while (!Open_BFS.empty())
    {
        Node *node = Open_BFS.front();
        Open_BFS.pop();
        Close_BFS.push(node);

        if (goal_check(node->state))
            return node;
        int opt;

        for (opt = 0; opt < 4; opt++)
        {
            State newState;
            makeNullState(&newState);
            xoay3ly(node->state, &newState, opt);

            if (findState(newState, Close_BFS) || findState(newState, Open_BFS))
                continue;
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->state = newState;
            newNode->parent = node;
            newNode->no_function = opt;
            Open_BFS.push(newNode);
        }
    }
    return NULL;
}

void print_WaytoGetGoal(Node *node)
{
    stack<Node *> stackPrint;

    while (node->parent != NULL)
    {
        stackPrint.push(node);
        node = node->parent;
    }
    stackPrint.push(node);
    int no_action = 0;
    while (!stackPrint.empty())
    {
        printf("\nChuyen x:%d x:%d x:%d\n", stackPrint.top()->no_function, stackPrint.top()->no_function + 1, stackPrint.top()->no_function + 2);
        print_state(stackPrint.top()->state);
        stackPrint.pop();
        no_action++;
    }
}

int main()
{
    State cur_state;
    makeNullState(&cur_state);
    Node *p = BFS_Algorithm(cur_state);
    print_WaytoGetGoal(p);
    return 0;
}