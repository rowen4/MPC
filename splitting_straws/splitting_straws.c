#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_shortest_odd(int* straws, int total_straws)
{
	int i = 0, shortest = INT_MAX;
	for (i = 0; i < total_straws; i++)
	{
		if (straws[i] < shortest && (straws[i] % 2))
		{
			shortest = straws[i];
		}
	}
	return shortest;
}

int step1(int* straws,int total_straws, int *next_slot)
{
	int i = 0;
	for (i = 0; i < total_straws; i++)
	{
		if (!(straws[i] % 2) && !((straws[i]/2) % 2))
		{
			straws[i] = straws[i] / 2;
			straws[*next_slot] = straws[i];
			(*next_slot)++;
		}
	}
	return 0;
}

int step2(int* straws, int total_straws, int *next_slot)
{
	int i = 0, s = 0, t = 0;
	int * remaining = malloc(sizeof(int) * total_straws);
	memset(remaining,0,sizeof(int) * total_straws);
	s = find_shortest_odd(straws, total_straws);
	if (s == 0)
		return 1;					//There are no odd-length straws remaining
	for (i = 0; i < total_straws; i++)
	{
		if (straws[i] % 2 && straws[i] > s)
		{
			straws[i] = straws[i] - s;
			straws[*next_slot] = s;
			remaining[i] = straws[i];		//This is necessary to differentiate from the even-length straws
			(*next_slot)++;
		}
	}
	for (i = 0; i < total_straws; i++)
	{
		if (remaining[i] > s)
		{
			free(remaining);
			return 0;				//At least one that was cut is of greater length than the shortest straw
		}
	}
	return 1;	
}

int main(int argc, char** argv)
{
	int i = 0, c = 0, starting_num = 0, next_slot = 0;
	int* straws;
	fscanf(stdin,"%d",&starting_num);
	straws = malloc(sizeof(int) * starting_num * 2);
	for (i = 0; i<starting_num;i++)
	{
		fscanf(stdin,"%d",(straws + i));
	}
	next_slot = starting_num;
	while(!c)
	{
		step1(straws,next_slot - 1, &next_slot);
		straws = realloc(straws, sizeof(int) * next_slot * 2);
		c = step2(straws, next_slot - 1, &next_slot);
		straws = realloc(straws, sizeof(int) * next_slot * 2);
	}
	fprintf(stdout,"%d",next_slot);
}
