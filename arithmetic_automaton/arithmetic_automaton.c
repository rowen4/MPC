#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char determine_new_state(int l, int  s, int r)
{
	int temp = 0;
	if (s < l && l < r)
		temp = abs(s * (l + r)) % 10;
	else if (s < r && r < l)
		temp = abs(s * (l - r)) % 10;
	else if (l < s && s < r)
		temp = abs(s + (l + r) * (l - r)) % 10;
	else if (l < r && r < s)
		temp = abs(s - (l + r) * (l - r)) % 10;
	else if (r < s && s < l)
		temp = abs(s + 2*l*r) % 10;
	else if (r < l && l < s)
		temp = abs(s - 2*l*r) % 10;
	else if (r == l && r != s)
		temp = s;
	else if (s == r && r != l)
		temp = l;
	else if (s == l && r != l)
		temp = r;
	else if (s == l && r == l)
		temp = abs(s + l + r) % 10;
	return temp;
}
int main(int argc, char** argv)
{
	char	dat_buff[65];
	int	length;
	int	steps;
	int 	i,j,l,r;
	unsigned char *old_data, *new_data, *temp;
	fscanf(stdin,"%s",dat_buff);
	length = strlen(dat_buff);
	old_data = malloc(sizeof(char) * length);
	new_data = malloc(sizeof(char) * length);
	for(i = 0; i < length; i++)
		old_data[i] = dat_buff[i] - '0';
	fscanf(stdin,"%s",dat_buff);
	steps = atoi(dat_buff);
	for(i = 0; i < steps; i++)
	{
		for(j = 0; j < length; j++)
		{
			if(j == 0)
				l = length-1;
			else
				l = j - 1;
			if(j == length - 1)
				r = 0;
			else
				r = j + 1;	
			new_data[j] = determine_new_state(old_data[l],old_data[j],old_data[r]);
		}
		temp = old_data;
		old_data = new_data;
		new_data = temp;
	}
	for(i = 0; i < length; i++)
		dat_buff[i] = old_data[i] + '0';
	printf("%s",dat_buff);
	return 0;
}
