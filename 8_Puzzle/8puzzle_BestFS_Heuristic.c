#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 3
#define Empty 0
#define Max_Operation 4
#define Maxlenght 1000

const char *action[] = {"First State", "Move cell Empty to Up", "Move cell Empty to Down",
                        "Move cell Empty to Left", "Move cell Empty to Right"};

typedef struct
{
    int eightPuzzle[ROWS][COLS];
    int emptyRows;
    int emptyCols;
} State;

void printState(State state)
{
    int row, col;
    printf("\n----------\n");
    for (row = 0; row < ROWS; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            printf("|%d ", state.eightPuzzle[row][col]);
        }
        printf("|\n");  
    }
    printf("----------\n");
}
int compareState(State s1, State s2)
{
    if (s1.emptyRows != s2.emptyRows || s1.emptyCols != s2.emptyCols)
        return 0;
    int row, col;
    for (row = 0; row < ROWS; row++){
    	for (col = 0; col < COLS; col++)
    	{
			if (s1.eightPuzzle[row][col] != s2.eightPuzzle[row][col])
                return 0;
		}
            
	}
    return 1;
}
int goalCheck(State s, State GOAL)
{
    return compareState(s, GOAL);
}
int upOperators(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRows, emptyColCurrent = state.emptyCols;
    if (emptyRowCurrent > 0)
    {
        result->emptyRows = emptyRowCurrent - 1;
        result->emptyCols = emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent - 1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent - 1][emptyColCurrent] = Empty;
        return 1;
    }
    return 0;
}

int DownOperators(State state, State *result)
{

    *result = state;
    int emptyRowCurrent = state.emptyRows, emptyColCurrent = state.emptyCols;
    if (emptyRowCurrent < 2)
    {
        result->emptyRows = emptyRowCurrent + 1;
        result->emptyCols = emptyColCurrent;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent + 1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent + 1][emptyColCurrent] = Empty;
        return 1;
    }
    return 0;
}

int leftOperators(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRows, emptyColCurrent = state.emptyCols;
    if (emptyColCurrent > 0)
    {
        result->emptyRows = emptyRowCurrent;
        result->emptyCols = emptyColCurrent - 1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent][emptyColCurrent - 1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent - 1] = Empty;
        return 1;
    }
    return 0;
}

int rightOperators(State state, State *result)
{
    *result = state;
    int emptyRowCurrent = state.emptyRows, emptyColCurrent = state.emptyCols;
    if (emptyColCurrent < 2)
    {
        result->emptyRows = emptyRowCurrent;
        result->emptyCols = emptyColCurrent + 1;
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = state.eightPuzzle[emptyRowCurrent][emptyColCurrent + 1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent + 1] = Empty;
        return 1;
    }
    return 0;
}

// dem so o sai khac voi o muc tieu
int heuristic1(State state, State goal)
{
    int row, col, count = 0;
    for (row = 0; row < ROWS; row++){
    	for (col = 0; col < COLS; col++){
    		if (state.eightPuzzle[row][col] != goal.eightPuzzle[row][col])
                count++;
		}
	}
        

    return count;
}

int heuristic2(State state, State goal)
{
    int count = 0;
    int row, col, row_g, col_g;
    for (row = 0; row < ROWS; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            if (state.eightPuzzle[row][col] != Empty)
            {
                for (row_g = 0; row_g < ROWS; row_g++)
                {
                    for (col_g = 0; col_g < COLS; col_g++)
                    {
                        if (state.eightPuzzle[row][col] == goal.eightPuzzle[row_g][col_g])
                        {
                            count += abs(row - row_g) + abs(col - col_g);
                            col_g = COLS;
                            row_g = ROWS;
                        }
                    }
                }
            }
        }
    }
    return count;
}

typedef struct Node
{
    State state;
    struct Node *parent;
    int no_function;
    int heuristic;
} Node;

typedef struct
{
    Node *Element[Maxlenght];
    int size;
} List;

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

Node *element_at(int p, List list)
{
    return list.Element[p - 1];
}

//void push_List(Node *x, int position, List *list)
//{
//    if (!full_List(*list))
//    {
//        int q;
//        for (q = list->size; q >= position; q--)
//        {
//            list->Element[q] = list->Element[q - 1];
//        }
//        list->Element[position - 1] = x;
//        list->size++;
//    }
//    else
//        printf("List is full!\n");
//}

void push_List(Node* x, int p, List* l){
	if(!full_List(*l)){
		int i;
		for(i = l->size; i >= p; i--){
			l->Element[i] = l->Element[i - 1];
		}
		l->Element[p - 1] = x;
		l->size++;
	} else {
		printf("FULL LIST!\n");
	}
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
        for (i = position - 1; i < list->size-1; i++)
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
        if (compareState(element_at(i, list)->state, state))
        {
            *position = i;
            return element_at(i, list);
        }
    }
    return NULL;
}

void sortList(List *list)
{
    int i, j;
    for (i = 0; i < list->size-1; i++)
    {
        for (j = i + 1; j < list->size; j++)
        {
            if (list->Element[i]->heuristic > list->Element[j]->heuristic)
            {
                Node *node = list->Element[i];
                list->Element[i] = list->Element[j];
                list->Element[j] = node;
            }
        }
    }
}

int call_operators(State state, State *result, int opt)
{
    switch (opt)
    {
    case 1:
        return upOperators(state, result);
    case 2:
        return DownOperators(state, result);
    case 3:
        return leftOperators(state, result);
    case 4:
        return rightOperators(state, result);
    default:
        printf("Cannot call operators");
        return 0;
    }
}

Node *best_first_search(State state, State goal)
{
    List Open_bestFS;
    List Close_bestFS;
    makeNullList(&Open_bestFS);
    makeNullList(&Close_bestFS);

    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    root->heuristic = heuristic1(root->state, goal);
    push_List(root, Open_bestFS.size + 1, &Open_bestFS);
    while (!emptyList(Open_bestFS))
    {
        Node *node = element_at(1, Open_bestFS);
        delete_List(1, &Open_bestFS);
        push_List(node, Close_bestFS.size + 1, &Close_bestFS);
        if (goalCheck(node->state, goal))
            return node;

        int opt;
        for (opt = 1; opt <= Max_Operation; opt++)
        {
            State newState;
            newState = node->state;
            if (call_operators(node->state, &newState, opt))
            {
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->no_function = opt;
                newNode->heuristic = heuristic1(newState, goal);
                
                int pos_Open, posClose;
                
                Node *nodeFoundOpen = findState(newState, Open_bestFS, &pos_Open);
                Node *nodeFoundClose = findState(newState, Close_bestFS, &posClose);
                
                if (nodeFoundOpen == NULL && nodeFoundClose == NULL)
                    push_List(newNode, Open_bestFS.size + 1, &Open_bestFS);
                else if (nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic)
                {
                    delete_List(pos_Open, &Open_bestFS);
                    push_List(newNode, pos_Open, &Open_bestFS);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic)
                {
                    delete_List(posClose, &Close_bestFS);
                    push_List(newNode, Open_bestFS.size+1, &Open_bestFS);
                }
                sortList(&Open_bestFS);
            }
        }
    }
    return NULL;
}

void print_WaytogetGoal(Node *node)
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
        printf("\nAction %d: %s", no_action, action[element_at(i, listPrint)->no_function]);
        printState(element_at(i, listPrint)->state);
        no_action++;
    }
}

int main()
{
    State state;
    state.emptyRows = 1;
    state.emptyCols = 1;
    state.eightPuzzle[0][0] = 3;
    state.eightPuzzle[0][1] = 4;
    state.eightPuzzle[0][2] = 5;
    state.eightPuzzle[1][0] = 1;
    state.eightPuzzle[1][1] = 0;
    state.eightPuzzle[1][2] = 2;
    state.eightPuzzle[2][0] = 6;
    state.eightPuzzle[2][1] = 7;
    state.eightPuzzle[2][2] = 8;

    State goal;
    goal.emptyRows = 0;
    goal.emptyCols = 0;
    goal.eightPuzzle[0][0] = 0;
    goal.eightPuzzle[0][1] = 1;
    goal.eightPuzzle[0][2] = 2;
    goal.eightPuzzle[1][0] = 3;
    goal.eightPuzzle[1][1] = 4;
    goal.eightPuzzle[1][2] = 5;
    goal.eightPuzzle[2][0] = 6;
    goal.eightPuzzle[2][1] = 7;
    goal.eightPuzzle[2][2] = 8;
    Node *p = best_first_search(state, goal);
    print_WaytogetGoal(p);
    return 0;
}
