#include <stdio.h>
#include <stdlib.h>
#include <stack>
#define MaxLength 100

using namespace std;

typedef struct
{
    int Priest;
    int DEL;
    char edge;
} State;

void makeNull(State *state)
{
    state->Priest = 3;
    state->DEL =
        3;
    state->edge = 'A';
}

typedef struct Node
{
    State state;
    struct Node *parent;
    int no_function;
} Node;

int MOVE_1_Priest(State current, State *result)
{
    if (current.edge == 'A' && current.Priest > 0)
    {
        result->Priest = current.Priest - 1;
        result->DEL = current.DEL;
        result->edge = 'B';
        return 1;
    }

    else if (current.edge == 'B' && current.Priest < 3)
    {
        result->Priest = current.Priest + 1;
        result->DEL = current.DEL;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

int MOVE_2_Priest(State current, State *result)
{
    if (current.edge == 'A' && current.Priest > 1)
    {
        result->Priest = current.Priest - 2;
        result->DEL = current.DEL;
        result->edge = 'B';
        return 1;
    }
    else if (current.edge == 'B' && current.Priest < 2)
    {
        result->Priest = current.Priest + 2;
        result->DEL = current.DEL;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

int MOVE_1_DEL(State current, State *result)
{
    if (current.edge == 'A' && current.DEL > 0)
    {
        result->DEL = current.DEL - 1;
        result->Priest = current.Priest;
        result->edge = 'B';
        return 1;
    }
    else if (current.edge == 'B' && current.DEL < 3)
    {
        result->DEL = current.DEL + 1;
        result->Priest = current.Priest;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

int MOVE_2_DEL(State current, State *result)
{
    if (current.edge == 'A' && current.DEL > 1)
    {
        result->DEL = current.DEL - 2;
        result->Priest = current.Priest;
        result->edge = 'B';
        return 1;
    }
    else if (current.edge == 'B' && current.DEL < 2)
    {
        result->DEL = current.DEL + 2;
        result->Priest = current.Priest;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

int MOVE_1DEL_1Priest(State current, State *result)
{
    if (current.edge == 'A' && current.Priest > 0 && current.DEL > 0)
    {
        result->Priest = current.Priest - 1;
        result->DEL = current.DEL - 1;
        result->edge = 'B';
        return 1;
    }
    else if (current.edge == 'B' && current.Priest < 3 && current.DEL < 3)
    {
        result->Priest = current.Priest + 1;
        result->DEL = current.DEL + 1;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

//
int check(State s)
{
    if (s.edge == 'A' && s.DEL > s.Priest && s.Priest != 0)
    {
        return 0;
    }
    else if (s.edge == 'B' && s.DEL < s.Priest && s.Priest != 3)
    {
        return 0;
    }
    return 1;
}
int call_operator(State current, State *result, int option)
{
    switch (option)
    {
    case 1:
        return MOVE_1_Priest(current, result);
    case 2:
        return MOVE_2_Priest(current, result);
    case 3:
        return MOVE_1_DEL(current, result);
    case 4:
        return MOVE_2_DEL(current, result);
    case 5:
        return MOVE_1DEL_1Priest(current, result);
    default:
        printf("ERROR");
    }
    return 0;
}

int compareStates(State *state1, State *state2)
{
    return (state1->Priest == state2->Priest) && (state1->DEL == state2->DEL) && (state1->edge == state2->edge);
}

int findState(State state, stack<Node *> openStack)
{
    while (!openStack.empty())
    {
        if (compareStates(&openStack.top()->state, &state))
        {
            return 1;
        }
        openStack.pop();
    }
    return 0;
}
void print_state(State state)
{
    printf("\nPriest:%d---DEL:%d---side:%c", state.Priest, state.DEL, state.edge);
}

int goal_check(State state)
{
    return state.Priest == 0 && state.DEL == 0 && state.edge == 'B';
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

const char *action[] = {"First State", "MOVE 1 Tu Si", "MOVE 2 Tu Si",
                        "MOVE 1 DEL", "MOVE 2 DEL", "MOVE 1 DEL va 1 Tu Si"};

Node *DFS_Algorithm(State state)
{
    stack<Node *> Open_DFS;
    stack<Node *> Close_DFS;

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    Open_DFS.push(root);

    while (!Open_DFS.empty())
    {
        Node *node = Open_DFS.top();
        Open_DFS.pop();
        Open_DFS.push(node);

        if (goal_check(node->state))
            return node;
        int opt;

        for (opt = 1; opt <= 5; opt++)
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
                Open_DFS.push(newNode);
            }
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
        printf("\nAction %d: %s", no_action, action[stackPrint.top()->no_function]);
        print_state(stackPrint.top()->state);
        stackPrint.pop();
        no_action++;
    }
}

int main()
{
    State current_state = {3, 3, 'A'};
    Node *p = DFS_Algorithm(current_state);
    print_WaytoGetGoal(p);
    return 0;
}