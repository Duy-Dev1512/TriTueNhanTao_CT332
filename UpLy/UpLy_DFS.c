#include <stdio.h>
#include <stdlib.h>

#define MaxLength 100
#define Max_Cup 6
typedef struct
{
    int A[Max_Cup];
} State;

void makeNullState(State *state)
{
    int i;
    for (i = 0; i < 6; i++)
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

typedef struct
{
    Node *Element[MaxLength];
    int Top_index;
} Stack;

int fullStack(Stack S)
{
    return S.Top_index == 0;
}

void makeNullStack(Stack *S)
{
    S->Top_index = MaxLength;
}

int emptyStack(Stack S)
{
    return S.Top_index == MaxLength;
}

Node *top(Stack S)
{
    if (!emptyStack(S))
        return S.Element[S.Top_index];
    return NULL;
}

void pop(Stack *S)
{
    if (!emptyStack(*S))
        S->Top_index += 1;
    else
        printf("ERROR! Stack is empty");
}
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

int findState(State state, Stack openStack)
{
    while (!emptyStack(openStack))
    {
        if (compareStates(&top(openStack)->state, &state))
        {
            return 1;
        }
        pop(&openStack);
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

void push_Stack(Node *x, Stack *S)
{
    if (fullStack(*S))
        printf("ERROR! Stack is full");
    else
    {
        S->Top_index -= 1;
        S->Element[S->Top_index] = x;
    }
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
Node *DFS_Algorithm(State state)
{
    Stack Open_DFS;
    Stack Close_DFS;
    makeNullStack(&Open_DFS);
    makeNullStack(&Close_DFS);

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    push_Stack(root, &Open_DFS);

    while (!emptyStack(Open_DFS))
    {
        Node *node = top(Open_DFS);
        pop(&Open_DFS);
        push_Stack(node, &Close_DFS);

        if (goal_check(node->state))
            return node;
        int opt;

        for (opt = 0; opt < 4; opt++)
        {
            State newState;
            makeNullState(&newState);
            xoay3ly(node->state, &newState, opt);

            if (findState(newState, Close_DFS) || findState(newState, Open_DFS))
                continue;
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->state = newState;
            newNode->parent = node;
            newNode->no_function = opt;
            push_Stack(newNode, &Open_DFS);
        }
    }
    return NULL;
}

void print_WaytoGetGoal(Node *node)
{
    Stack stackPrint;
    makeNullStack(&stackPrint);

    while (node->parent != NULL)
    {
        push_Stack(node, &stackPrint);
        node = node->parent;
    }
    push_Stack(node, &stackPrint);
    int no_action = 0;
    while (!emptyStack(stackPrint))
    {
        printf("\nChuyen x:%d x:%d x:%d\n", top(stackPrint)->no_function, top(stackPrint)->no_function + 1, top(stackPrint)->no_function + 2);
        print_state(top(stackPrint)->state);
        pop(&stackPrint);
        no_action++;
    }
}

int main()
{
    State cur_state;
    makeNullState(&cur_state);
    Node *p = DFS_Algorithm(cur_state);
    print_WaytoGetGoal(p);
    return 0;
}