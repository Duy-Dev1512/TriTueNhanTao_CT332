#include <stdio.h>
#include <stdlib.h>

#define MaxLength 100
typedef struct
{
    int A[6];
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

//    for (i = 0; i < 6; i++)
//    {
//        result->A[i] = current.A[i];
//    }
	*result = current;
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
        del_Queue(&Open_BFS);
        push_Queue(node, &Close_BFS);

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
            push_Queue(newNode, &Open_BFS);
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
        printf("\nState %d\n", top(stackPrint)->no_function);
        print_state(top(stackPrint)->state);
        pop(&stackPrint);
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
