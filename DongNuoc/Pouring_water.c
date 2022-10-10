#include <stdio.h>

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

int main()
{
	State current = {5, 4}, result;
	printf("Trang Thai Bat Dau\n");
	print_state(current);
	int opt;
	for (opt = 1; opt <= 6; opt++)
	{
		int thuchien = call_operator(current, &result, opt);
		if (thuchien == 1)
		{
			printf("\nHanh dong %s thuc hien thanh cong", action[opt]);
			print_state(result);
		}
		else
			printf("\nHanh dong %s thuc hien KHONG thanh cong", action[opt]);
	}
	return 0;
}


