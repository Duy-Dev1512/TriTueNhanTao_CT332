#include <stdio.h>
#include <stdlib.h>
#define succhua_x 9
#define succhua_y 4
#define empty 0
#define GOAL 6
#define MaxLength 100

typedef struct
{
    int x;
    int y;
} State;

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
    return (state1->x == state2->x)&&(state1->y == state2->y);
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

void makeNull(State *state)
{
    state->x = 0;
    state->y = 0;
}

void print_state(State state)
{
    printf("\n  X:%d---Y:%d", state.x, state.y);
}

int goal_check(State state)
{
    return (state.x == GOAL || state.y == GOAL);
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int pourFullWater_X(State current, State *result)
{
    if (current.x < succhua_x)
    {
        result->x = succhua_x;
        result->y = current.y;
        return 1;
    }
    return 0;
}

int pourFullWater_Y(State current, State *result)
{
    if (current.y < succhua_y)
    {
        result->x = current.x;
        result->y = succhua_y;
        return 1;
    }
    return 0;
}

int make_emptyX(State current, State *result)
{
    if (current.x > 0)
    {
        result->x = empty;
        result->y = current.y;
        return 1;
    }
    return 0;
}

int make_emptyY(State current, State *result)
{
    if (current.y > 0)
    {
        result->x = current.x;
        result->y = empty;
        return 1;
    }
    return 0;
}

int pourXtoY(State current, State *result)
{
    if (current.x > 0 && current.y < succhua_y)
    {
        result->x = max(current.x - (succhua_y - current.y), empty);
        result->y = min(current.x + current.y, succhua_y);
        return 1;
    }
    return 0;
}

int pourYtoX(State current, State *result)
{
    if (current.y > 0 && current.x < succhua_x)
    {
        result->x = min(current.x + current.y, succhua_x);
        result->y = max(current.y - (succhua_x - current.x), empty);
        return 1;
    }
    return 0;
}

int call_operator(State current, State *result, int option)
{
    switch (option)
    {
    case 1:
        return pourFullWater_X(current, result);
    case 2:
        return pourFullWater_Y(current, result);
    case 3:
        return make_emptyX(current, result);
    case 4:
        return make_emptyY(current, result);
    case 5:
        return pourXtoY(current, result);
    case 6:
        return pourYtoX(current, result);
    default:
        printf("ERROR");
        return 0;
    }
}

const char *action[] = {"First State", "Pour Water Full X", "Pour Water Full Y",
                        "Make Empty X", "Make Empty Y", "Pour Water X to Y", "Pour Water Y to X"};

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

        for (opt = 1; opt <= 6; opt++)
        {
            State newState;
            makeNull(&newState);
            if (call_operator(node->state, &newState, opt))
            {

                if (findState(newState, Close_DFS) || findState(newState, Open_DFS))
                    continue;
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->no_function = opt;
                push_Stack(newNode, &Open_DFS);
            }
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
        printf("\nAction %d: %s", no_action, action[top(stackPrint)->no_function]);
        print_state(top(stackPrint)->state);
        pop(&stackPrint);
        no_action++;
    }
}
int main()
{
    State current_state = {0, 0};
    Node *p = DFS_Algorithm(current_state);
    print_WaytoGetGoal(p);
    return 0;
}
