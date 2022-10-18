#include <stdio.h>
#include <stdlib.h>

#define Maxlenght 1000
#define Max_Vertices 5

const char name[] = {'A', 'B', 'C', 'D', 'G'};

typedef struct
{
    int neighbor[Max_Vertices];
    int h;
} Vertices;

typedef struct
{
    int num_vertices;
    Vertices v[Max_Vertices];
} Graph;

typedef struct
{
    int vertex;
} State;

void printState(State state)
{
    printf("%c", name[state.vertex]);
}

int compareStates(State s1, State s2)
{
    return s1.vertex == s2.vertex;
}

typedef struct Node
{
    State state;
    struct Node *parent;
    int f, g, h;
} Node;

typedef struct
{
    Node *Element[Maxlenght];
    int size;
} List;

void init_graph(int num_ver, Graph *G)
{
    G->num_vertices = num_ver;
    int i, j;
    for (i = 0; i < num_ver; i++)
    {
        for (j = 0; j < num_ver; j++)
        {
            G->v[i].neighbor[j] = 0;
            G->v[i].h = 0;
        }
    }
}

void makeNullList(List *pL)
{
    pL->size = 0;
}

int emptyList(List L)
{
    return L.size == 0;
}

int full_List(List L)
{
    return L.size == Maxlenght;
}

void sortList(List *list)
{
    int i, j;
    for (i = 0; i < list->size - 1; i++)
    {
        for (j = i + 1; j < list->size; j++)
        {
            if (list->Element[i]->f > list->Element[j]->f)
            {
                Node *node = list->Element[i];
                list->Element[i] = list->Element[j];
                list->Element[j] = node;
            }
        }
    }
}
Node *element_at(int p, List list)
{
    return list.Element[p - 1];
}

void push_List(Node *x, int position, List *list)
{
    if (!full_List(*list))
    {
        int q;
        for (q = list->size; q >= position; q--)
        {
            list->Element[q] = list->Element[q - 1];
        }
        list->Element[position - 1] = x;
        list->size++;
    }
    else
        printf("List is full!\n");
}

void delete_List(int position, List *list)
{
    if (emptyList(*list))
        printf("List is empty!\n");
    else if (position < 1 || position > list->size)
        printf("Position is not possible to delete!\n");
    else
    {
        int i;
        for (i = position - 1; i < list->size - 1; i++)
        {
            list->Element[i] = list->Element[i + 1];
        }
        list->size--;
    }
}

Node *findState(State state, List list, int *position)
{
    int i;
    for (i = 1; i <= list.size; i++)
    {
        if (compareStates(element_at(i, list)->state, state))
        {
            *position = i;
            return element_at(i, list);
        }
    }
    return NULL;
}

int goalCheck(State s, State GOAL)
{
    return compareStates(s, GOAL);
}

Node *A_Star(Graph G, State state, State goal)
{
    List Open_A_Star;
    List Close_A_Star;
    makeNullList(&Open_A_Star);
    makeNullList(&Close_A_Star);

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->g = 0;
    root->h = G.v[state.vertex].h;
    root->f = root->g + root->h;
    push_List(root, Open_A_Star.size + 1, &Open_A_Star);

    while (!emptyList(Open_A_Star))
    {
        Node *node = element_at(1, Open_A_Star);
        delete_List(1, &Open_A_Star);
        push_List(node, Close_A_Star.size + 1, &Close_A_Star);
        if (goalCheck(node->state, goal))
        {
            return node;
        }
        int opt;
        for (opt = 0; opt < G.num_vertices; opt++)
        {
            if (G.v[node->state.vertex].neighbor[opt] > 0)
            {
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state.vertex = opt;
                newNode->parent = node;
                newNode->g = node->g + G.v[node->state.vertex].neighbor[opt];
                newNode->h = G.v[opt].h;
                newNode->f = newNode->g + newNode->h;
                int pos_Open = 0, pos_Close = 0;
                Node *nodeFoundOpen = findState(newNode->state, Open_A_Star, &pos_Open);
                Node *nodeFoundClose = findState(newNode->state, Close_A_Star, &pos_Close);

                if (nodeFoundOpen == NULL && nodeFoundClose == NULL)
                    push_List(newNode, Open_A_Star.size + 1, &Open_A_Star);
                else if (nodeFoundOpen != NULL && nodeFoundOpen->g > newNode->g)
                {
                    delete_List(pos_Open, &Open_A_Star);
                    push_List(newNode, pos_Open, &Open_A_Star);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->g > newNode->g)
                {
                    delete_List(pos_Close, &Close_A_Star);
                    push_List(newNode, Open_A_Star.size + 1, &Open_A_Star);
                }
            }
        }
        sortList(&Open_A_Star);
    }
    return NULL;
}

void printWayToGetGoal(Node *node)
{
    List listPrint;
    makeNullList(&listPrint);
    while (node->parent)
    {
        push_List(node, listPrint.size + 1, &listPrint);
        node = node->parent;
    }
    push_List(node, listPrint.size + 1, &listPrint);

    int no_action = 0, i;
    for (i = listPrint.size; i > 0; i--)
    {
        printState(element_at(i, listPrint)->state);
        if (i != 1)
            printf("->");
        no_action++;
    }
}
int main()
{
    int i, j;
    Graph graph;
    freopen("test.txt", "r", stdin);
    init_graph(Max_Vertices, &graph);
    for (i = 0; i < Max_Vertices; i++)
    {
        int x;
        scanf("%d", &x);
        graph.v[i].h = x;
        for (j = 0; j < Max_Vertices; j++)
        {
            scanf("%d", &x);
            graph.v[i].neighbor[j] = x;
        }
    }
    State A, G;
    A.vertex = 0;
    G.vertex = 4;
    Node *result = A_Star(graph, A, G);
    if (result)
        printWayToGetGoal(result);
    return 0;
}