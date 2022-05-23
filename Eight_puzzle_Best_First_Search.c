#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

int GoalState[9], CurrentState[9], ChildState[9] = {0,0,0,0,0,0,0,0,0};

void GetGoalState(int initialstate[], int goalstate[])
{
    int i, j, total = 0;
    int GoalStateType1[] = {1,2,3,8,0,4,7,6,5};
    int GoalStateType2[] = {0,1,2,3,4,5,6,7,8};
    for(i=0; i<8; i++)
	{
        for(j=i+1; j<9; j++)
		{
            if(initialstate[j] < initialstate[i] && initialstate[j] != 0)
                total = total + 1;  
        }
    }
    if(total % 2 == 1)
	{
        for(i=0; i<9; i++)
		{
            goalstate[i] = GoalStateType1[i];
        }
    }
	else
	{
        for(i=0; i<9; i++)
		{
            goalstate[i] = GoalStateType2[i];
        }
    }
}

void PrintState(int state[])
{
	int i;
    for(i=0; i<9; i++)
	{
        if(i % 3 == 0)
            printf("\n");
        printf(" %d", state[i]);
    }
	printf("\n");
}

void CopyState(int from[], int to[])
{
	int i;
    for(i=0; i<9; i++)
	{
        to[i] = from[i];
    }
}

bool SameState(int currentstate[], int goalstate[])
{
	int i;
    for(i=0; i<9; i++)
	{
        if(currentstate[i] != goalstate[i])
		{
            return false;
            break;
        }
    }
    return true;
}

int GetCurrentState(int state[])
{
    int i, index = 0;
    for(i=0; i<9; i++)
	{
        if(state[i] == 0)
            index = i;
    }
    return index;
}

void MoveUp(int state[])
{
    int index = GetCurrentState(state);
    if(index > 2)
	{      
        state[index] = state[index-3];
        state[index-3] = state[index-3] - state[index];
    }
}

void MoveDown(int state[])
{
    int index = GetCurrentState(state);
    if(index < 6)
	{
        state[index] = state[index+3];
        state[index+3] = state[index+3] - state[index];
    }
}

void MoveLeft(int state[])
{
    int index = GetCurrentState(state);
    if(index % 3 > 0)
	{
        state[index] = state[index-1];
        state[index-1] = state[index-1] - state[index];
    }
}

void MoveRight(int state[])
{
    int index = GetCurrentState(state);
    if(index % 3 < 2)
	{
        state[index] = state[index+1];
        state[index+1] = state[index+1] - state[index];
    }
}

int MatchTile(int currentstate[], int goalstate[])
{
    int i, match = 0;
    for(i=0; i<9; i++)
	{
        if(currentstate[i] != 0 && currentstate[i] == goalstate[i])
            match++;
    }
    return match;
}

int GetTheBestMove(int heuristic[])
{
    int i, index = 0;
    int max = heuristic[0];
    for(i=0; i<4; i++)
	{
        if(heuristic[i] > max)
		{
            max = heuristic[i];
            index = i;
        }
    }
    return index;
}

int main(int argc, char** argv) 
{
    int InitialState[] = {2,8,3,1,6,4,7,0,5};
    GetGoalState(InitialState,GoalState);
    printf("\nInit State\n----------");        
    PrintState(InitialState);
    printf("\nGoal State\n----------");
	PrintState(GoalState);
    printf("\nSearching\n----------");
	CopyState(InitialState,CurrentState);
    int level = 0;
    clock_t t;
    t = clock();
    while(!SameState(CurrentState,GoalState))
	{
        PrintState(CurrentState);
        CopyState(CurrentState,ChildState);
        int Heuristic[] = {0,0,0,0};
        level++;
        MoveUp(ChildState);
        if(!SameState(ChildState,CurrentState))
		{
            Heuristic[0] = MatchTile(ChildState,GoalState) + level;
            MoveDown(ChildState);
        }
        MoveDown(ChildState);
        if(!SameState(ChildState,CurrentState))
		{
            Heuristic[1] = MatchTile(ChildState,GoalState) + level;
            MoveUp(ChildState);
        }
        MoveLeft(ChildState);
        if(!SameState(ChildState,CurrentState))
		{
            Heuristic[2] = MatchTile(ChildState,GoalState) + level;
            MoveRight(ChildState);
        }
        MoveRight(ChildState);
        if(!SameState(ChildState,CurrentState))
		{
            Heuristic[3] = MatchTile(ChildState,GoalState) + level;
            MoveLeft(ChildState);
        }
        switch(GetTheBestMove(Heuristic))
		{
            case 0 : MoveUp(CurrentState); break;
            case 1 : MoveDown(CurrentState); break;
            case 2 : MoveLeft(CurrentState); break;
            case 3 : MoveRight(CurrentState); break;
        }       
    }
    PrintState(CurrentState);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("\n Solved with %i steps, %f seconds...",level,time_taken);
    getchar();
    return 0;
}

