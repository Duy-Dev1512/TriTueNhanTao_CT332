#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>

using namespace std;

#define succhua_x 9
#define succhua_y 4
#define Empty 0
#define MaxLength 100
#define GOAL 6

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
        result->x = Empty;
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
        result->y = Empty;
        return 1;
    }
    return 0;
}

int pourXtoY(State current, State *result)
{
    if (current.x > 0 && current.y < succhua_y)
    {
        result->x = max(current.x - (succhua_y - current.y), Empty);
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
        result->y = max(current.y - (succhua_x - current.x), Empty);
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

int compareStates(State *state1, State *state2)
{
    return (state1->x == state2->x) && (state1->y == state2->y);
}

int findStates(State state, queue<Node *> openQueue)
{
    while (!openQueue.empty())
    {
        if (compareStates(&openQueue.front()->state, &state))
            return 1;
        openQueue.pop();
    }
    return 0;
}

Node *BFS_Algorithm(State state)
{
    queue<Node *> OpenBFS;
    queue<Node *> CloseBFS;

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    OpenBFS.push(root);

    while (!OpenBFS.empty())
    {
        Node *node = OpenBFS.front();
        OpenBFS.pop();
        CloseBFS.push(node);

        if (goal_check(node->state))
            return node;
        int opt;

        for (opt = 1; opt <= 6; opt++)
        {
            State newState;
            makeNull(&newState);
            if (call_operator(node->state, &newState, opt))
            {
                if (findStates(newState, CloseBFS) || findStates(newState, OpenBFS))
                    continue;

                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->no_function = opt;
                OpenBFS.push(newNode);
            }
        }
    }
    return NULL;
}

void printWaytoGetGoal(Node *node)
{
    queue<Node *> queuePrint;
    stack<Node *> swapStack;
    while (node->parent != NULL)
    {
        queuePrint.push(node);
        node = node->parent;
    }
    queuePrint.push(node);
    int no_action = 0;
    while(!queuePrint.empty()){
        swapStack.push(queuePrint.front());
        queuePrint.pop();
    }

    while (!swapStack.empty())
    {
        printf("\nAction %d: %s", no_action, action[swapStack.top()->no_function]);
        print_state(swapStack.top()->state);
        swapStack.pop();
        no_action++;
    }
}

int main()
{
    State current_state = {0,0};
    Node *p = BFS_Algorithm(current_state);
    printWaytoGetGoal(p);
    return 0;
}