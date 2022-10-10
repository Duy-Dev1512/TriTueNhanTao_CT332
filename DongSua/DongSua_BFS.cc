#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>

using namespace std;

#define MAX_X 10
#define MAX_Y 5
#define MAX_Z 6
#define Empty 0
#define GOAL 8
#define MaxLength 100

typedef struct
{
    int x;
    int y;
    int z;
} State;


void makeNull(State *state)
{
    state->x = 0;
    state->y = 0;
    state->z = 0;
}

typedef struct Node
{
    State state;
    struct Node *parent;
    int no_function;
} Node;



int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int goal_check(State state)
{
    return (state.x == GOAL || state.y == GOAL || state.z == GOAL);
}


int compareStates(State *state1, State *state2)
{
    return (state1->x == state2->x) && (state1->y == state2->y) && (state1->z == state2->z);
}

int findState(State state, queue<Node *> openQueue)
{
    while (!openQueue.empty())
    {
        if (compareStates(&openQueue.front()->state, &state))
            return 1;
        openQueue.pop();
    }
    return 0;
}

int pourXtoY(State current, State *result)
{
    if (current.x > 0 && current.y < MAX_Y)
    {
        result->x = max(current.x - (MAX_Y - current.y), Empty);
        result->y = min(current.x + current.y, MAX_Y);
        result->z = current.z;
        return 1;
    }
    return 0;
}

int pourXtoZ(State current, State *result)
{
    if (current.x > 0 && current.z < MAX_Z)
    {
        result->x = max(current.x - (MAX_Z - current.z), Empty);
        result->y = current.y;
        result->z = min(current.x + current.z, MAX_Z);
        return 1;
    }
    return 0;
}

int pourYtoX(State current, State *result)
{
    if (current.y > 0 && current.x < MAX_X)
    {
        result->x = min(current.x + current.y, MAX_X);
        result->y = max(current.y - (MAX_X - current.x), Empty);
        result->z = current.z;
        return 1;
    }
    return 0;
}
int pourYtoZ(State current, State *result)
{
    if (current.y > 0 && current.z < MAX_Z)
    {
        result->x = current.x;
        result->y = max(current.y - (MAX_Z - current.z), Empty);
        result->z = min(current.z + current.y, MAX_Z);
        return 1;
    }
    return 0;
}

int pourZtoX(State current, State *result)
{
    if (current.z > 0 && current.x < MAX_X)
    {
        result->x = min(current.x + current.z, MAX_X);
        result->y = current.y;
        result->z = max(current.z - (MAX_X - current.x), Empty);
        return 1;
    }
    return 0;
}

int pourZtoY(State current, State *result){
    if(current.z > 0 && current.y < MAX_Y){
        result->x = current.x;
        result->y = min(current.y + current.z, MAX_Y);
        result->z = max(current.z - (MAX_Y - current.y), Empty);
        return 1;
    }
    return 0;
}

int call_operator(State current, State *result, int option)
{
    switch (option)
    {
    case 1:
        return pourXtoY(current, result);
    case 2:
        return pourXtoZ(current, result);
    case 3:
        return pourYtoX(current, result);
    case 4:
        return pourYtoZ(current, result);
    case 5:
        return pourZtoX(current, result);
    case 6:
        return pourZtoY(current, result);
    default:
        printf("ERROR");
        return 0;
    }
}

const char *action[] = {"First State", "PourXtoY", "PourXtoZ",
                        "PourYtoX", "PourYtoZ", "PourZtoX", "PourZtoY"};




void print_state(State state)
{
    printf("\nX:%d---Y:%d---Z:%d", state.x, state.y, state.z);
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
                if (findState(newState, CloseBFS) || findState(newState, OpenBFS))
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



int main(){
    State current_state = {10, 0, 0};
    Node *p = BFS_Algorithm(current_state);
    printWaytoGetGoal(p);
    return 0;
}