#include <stdio.h>
#include <stdlib.h>
#include <stack>

#define Max_Lenght 100
#define Max_Cup 6
using namespace std;

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

int findState(State state, stack<Node *> openStack)
{
    while (!openStack.empty())
    {
        if (compareStates(&state, &openStack.top()->state))
            return 1;
        openStack.pop();
    }
    return 0;
}

int goal_check(State state)
{
    int i;
    for (i = 0; i < Max_Cup; i++)
    {
        if (state.A[i] != 1)
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
    stack<Node *> OpenStack;
    stack<Node *> CloseStack;

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;

    OpenStack.push(root);
    while (!OpenStack.empty())
    {
        Node *node = OpenStack.top();
        OpenStack.pop();
        CloseStack.push(node);

        if (goal_check(node->state))
            return node;
        int opt;

        for (opt = 0; opt < 4; opt++)
        {
            State newState;
            makeNullState(&newState);
            xoay3ly(node->state, &newState, opt);

            if (findState(newState, CloseStack) || findState(newState, OpenStack))
                continue;

            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->state = newState;
            newNode->parent = node;
            newNode->no_function = opt;
            OpenStack.push(newNode);
        }
    }
    return NULL;
}

void printWaytoGetGoal(Node *node)
{
    stack<Node *> StackPrint;
    while (node->parent != NULL)
    {
        StackPrint.push(node);
        node = node->parent;
    }
    StackPrint.push(node);
    int no_action = 0;
    while (!StackPrint.empty())
    {
        printf("\n Chuyen x:%d x:%d x:%d\n", StackPrint.top()->no_function, StackPrint.top()->no_function + 1, StackPrint.top()->no_function + 2);
        print_state(StackPrint.top()->state);
        StackPrint.pop();
        no_action++;
    }
}

int main()
{
    State cur_state;
    makeNullState(&cur_state);
    Node *p = BFS_Algorithm(cur_state);
    printWaytoGetGoal(p);
    return 0;
}