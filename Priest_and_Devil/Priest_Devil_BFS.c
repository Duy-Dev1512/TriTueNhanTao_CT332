#include <stdio.h>
#include <stdlib.h>

#define MaxLength 100

typedef struct
{
    int TS;
    int QUY;
    char edge;
} State;

void makeNull(State *state)
{
    state->TS = 3;
    state->QUY = 3;
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
    int front, rear;
} Queue;

void makeNullQueue(Queue *queue)
{
    queue->front = -1;
    queue->rear = -1;
}

int emptyQueue(Queue queue)
{
    return queue.front == -1;
}

int full_queue(Queue queue)
{
    return ((queue.rear - queue.front + 1) % MaxLength) == 0;
}

Node *get_Front(Queue q)
{
    if (emptyQueue(q))
        printf("Queue is empty");
    else
        return q.Element[q.front];
}

void del_Queue(Queue *Q)
{
    if (!emptyQueue(*Q))
    {
        if (Q->front == Q->rear)
            makeNullQueue(Q);
        else
            Q->front = (Q->front + 1) % MaxLength;
    }
    else
        printf("ERROR, Delete");
}

void push_Queue(Node *x, Queue *Q)
{
    if (!full_queue(*Q))
    {
        if (emptyQueue(*Q))
            Q->front = 0;
        Q->rear = (Q->rear + 1) % MaxLength;
        Q->Element[Q->rear] = x;
    }
    else
        printf("ERROR, Push");
}

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
        printf("ERROR!Stack is empty");
}

int chuyen_1_TuSi(State current, State *result)
{
    if (current.edge == 'A' && current.TS > 0)
    {
        result->TS = current.TS - 1;
        result->QUY = current.QUY;
        result->edge = 'B';
        return 1;
    }

    else if (current.edge == 'B' && current.TS < 3)
    {
        result->TS = current.TS + 1;
        result->QUY = current.QUY;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

int chuyen_2_TuSi(State current, State *result)
{
    if (current.edge == 'A' && current.TS > 1)
    {
        result->TS = current.TS - 2;
        result->QUY = current.QUY;
        result->edge = 'B';
        return 1;
    }
    else if (current.edge == 'B' && current.TS < 2)
    {
        result->TS = current.TS + 2;
        result->QUY = current.QUY;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

int chuyen_1_Quy(State current, State *result)
{
    if (current.edge == 'A' && current.QUY > 0)
    {
        result->QUY = current.QUY - 1;
        result->TS = current.TS;
        result->edge = 'B';
        return 1;
    }
    else if (current.edge == 'B' && current.QUY < 3)
    {
        result->QUY = current.QUY + 1;
        result->TS = current.TS;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

int chuyen_2_Quy(State current, State *result)
{
    if (current.edge == 'A' && current.QUY > 1)
    {
        result->QUY = current.QUY - 2;
        result->TS = current.TS;
        result->edge = 'B';
        return 1;
    }
    else if (current.edge == 'B' && current.QUY < 2)
    {
        result->QUY = current.QUY + 2;
        result->TS = current.TS;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

int chuyen_1Quy_1TuSi(State current, State *result)
{
    if (current.edge == 'A' && current.TS > 0 && current.QUY > 0)
    {
        result->TS = current.TS - 1;
        result->QUY = current.QUY - 1;
        result->edge = 'B';
        return 1;
    }
    else if (current.edge == 'B' && current.TS < 3 && current.QUY < 3)
    {
        result->TS = current.TS + 1;
        result->QUY = current.QUY + 1;
        result->edge = 'A';
        return 1;
    }
    return 0;
}

//
int kiemtra(State s)
{
    if (s.edge == 'A' && s.QUY > s.TS && s.TS != 0)
    {
        return 0;
    }
    else if (s.edge == 'B' && s.QUY < s.TS && s.TS != 3)
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
        return chuyen_1_TuSi(current, result);
    case 2:
        return chuyen_2_TuSi(current, result);
    case 3:
        return chuyen_1_Quy(current, result);
    case 4:
        return chuyen_2_Quy(current, result);
    case 5:
        return chuyen_1Quy_1TuSi(current, result);
    default:
        printf("ERROR");
        return 0;
    }
}

int compareStates(State *state1, State *state2)
{
    return (state1->TS == state2->TS) && (state1->QUY == state2->QUY) && (state1->edge == state2->edge);
}

int findState(State state, Queue openQueue)
{
    while (!emptyQueue(openQueue))
    {
        if (compareStates(&get_Front(openQueue)->state, &state))
        {
            return 1;
        }
        del_Queue(&openQueue);
    }
    return 0;
}

void print_state(State state)
{
    printf("\nTS:%d---QUY:%d---Bo:%c", state.TS, state.QUY, state.edge);
}

int goal_check(State state)
{
    return state.TS == 0 && state.QUY == 0 && state.edge == 'B';
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

const char *action[] = {"First State", "Chuyen 1 Tu Si", "Chuyen 2 Tu Si",
                        "Chuyen 1 Con Quy", "Chuyen 2 Con Quy", "Chuyen 1 Con Quy va 1 Tu Si"};

Node *BFS_Algorithm(State state)
{
    Queue Open_BFS;
    Queue Close_BFS;
    makeNullQueue(&Open_BFS);
    makeNullQueue(&Close_BFS);

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    push_Queue(root, &Open_BFS);

    while (!emptyQueue(Open_BFS))
    {
        Node *node = get_Front(Open_BFS);
        push_Queue(node, &Close_BFS);
        del_Queue(&Open_BFS);

        if (goal_check(node->state))
            return node;
        int opt;

        for (opt = 1; opt <= 5; opt++)
        {
            State newState;
            makeNull(&newState);
            if (call_operator(node->state, &newState, opt))
            {
                if (findState(newState, Close_BFS) || findState(newState, Open_BFS))
                    continue;
                if (!kiemtra(newState))
                {
                    continue;
                }
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->no_function = opt;
                push_Queue(newNode, &Open_BFS);
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
    Node *p = BFS_Algorithm(current_state);
    print_WaytoGetGoal(p);
    return 0;
}