#include <stdio.h>
#include <stdlib.h>

#define MAX_X 10
#define MAX_Y 5
#define MAX_Z 6
#define empty 0
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
        printf("ERROR! Stack is empty");
}


int compareStates(State *state1, State *state2)
{
    return (state1->x == state2->x) && (state1->y == state2->y) && (state1->z == state2->z);
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
    printf("\nX:%d---Y:%d---Z:%d", state.x, state.y, state.z);
}

int goal_check(State state)
{
    return (state.x == GOAL || state.y == GOAL || state.z == GOAL);
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}


int pourXtoY(State current, State *result)
{
    if (current.x > 0 && current.y < MAX_Y)
    {
        result->x = max(current.x - (MAX_Y - current.y), empty);
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
        result->x = max(current.x - (MAX_Z - current.z), empty);
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
        result->y = max(current.y - (MAX_X - current.x), empty);
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
        result->y = max(current.y - (MAX_Z - current.z), empty);
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
        result->z = max(current.z - (MAX_X - current.x), empty);
        return 1;
    }
    return 0;
}

int pourZtoY(State current, State *result){
    if(current.z > 0 && current.y < MAX_Y){
        result->x = current.x;
        result->y = min(current.y + current.z, MAX_Y);
        result->z = max(current.z - (MAX_Y - current.y), empty);
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

        for (opt = 1; opt <= 6; opt++)
        {
            State newState;
            makeNull(&newState);
            if (call_operator(node->state, &newState, opt))
            {

                if (findState(newState, Close_BFS) || findState(newState, Open_BFS))
                    continue;
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
    Queue queuePrint;
    makeNullQueue(&queuePrint);
    Stack SwapStack;
    makeNullStack(&SwapStack);

    while (node->parent != NULL)
    {
        push_Queue(node, &queuePrint);
        node = node->parent;
    }
    push_Queue(node, &queuePrint);
    int no_action=0;

    while(!emptyQueue(queuePrint)){
        push_Stack(get_Front(queuePrint), &SwapStack);
        del_Queue(&queuePrint);
    }
    while(!emptyStack(SwapStack)){
        printf("\nAction %d: %s", no_action, action[top(SwapStack)->no_function]);
        print_state(top(SwapStack)->state);
        pop(&SwapStack);
        no_action++;
    }
 
}


int main(){
    State current_state = {10,0,0};
    Node *p = BFS_Algorithm(current_state);
    print_WaytoGetGoal(p);
    return 0;
}