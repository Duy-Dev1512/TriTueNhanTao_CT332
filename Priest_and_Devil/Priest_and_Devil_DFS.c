#include <stdio.h>
#include <stdlib.h>

#define MaxLength 100

typedef struct
{
    int Priest;
    int DEL;
    char edge;
} State;

void makeNull(State *state)
{
    state->Priest = 3;
    state->DEL = 3;
    state->edge = 'A';
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

int MOVE_1_PRIEST(State current, State *result)
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

int MOVE_2_PRIEST(State current, State *result)
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

int MOVE_1DEL_1PRIEST(State current, State *result)
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
int kiemtra(State s)
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
        return MOVE_1_PRIEST(current, result);
    case 2:
        return MOVE_2_PRIEST(current, result);
    case 3:
        return MOVE_1_DEL(current, result);
    case 4:
        return MOVE_2_DEL(current, result);
    case 5:
        return MOVE_1DEL_1PRIEST(current, result);
    default:
        printf("ERROR");
        return 0;
    }
    
}

int compareStates(State *state1, State *state2)
{
    return (state1->Priest == state2->Priest) && (state1->DEL == state2->DEL) && (state1->edge == state2->edge);
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

const char *action[] = {"First State", "MOVE 1 PRIEST", "MOVE 2 PRIEST",
                        "MOVE 1 DEL", "MOVE 2 DEL", "MOVE 1 DEL & 1 PRIEST"};


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
    State current_state = {3, 3, 'A'};
    Node *p = DFS_Algorithm(current_state);
    print_WaytoGetGoal(p);
    return 0;
}