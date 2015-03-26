#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char* base_key;
unsigned char* matrix_key;
unsigned char* clear_data;
unsigned char* encoded_data;
int blocks_read;

unsigned char crotl(unsigned char c, int s) {
    return (c << s) | (c >> ((sizeof(char) * 8) - s));
}

void read_data()
{
	int i = 0;
	char* c = NULL;
	base_key = malloc(sizeof(char)*8);		//Standard key size
	fscanf(stdin,"%hhX %hhX %hhX %hhX %hhX %hhX %hhX %hhX\n",&base_key[0],&base_key[1],&base_key[2],&base_key[3],&base_key[4],&base_key[5],&base_key[6],&base_key[7]);
	do
	{
		clear_data = realloc(clear_data,(blocks_read +1) * sizeof(char) * 48);	//Allocate room for a new block of data
		memset(clear_data + (blocks_read * 48),0,48);				//Zero out the new block
		c = fgets(clear_data + (blocks_read * 48),48,stdin);
		if (c != NULL)
			blocks_read++;
	}
	while(!feof(stdin));
	if (blocks_read > 0)
	{
		for (i = 1; i<=48;i++)
		{
			if (*(clear_data + (blocks_read*48) - i) == '\n')
			{
				*(clear_data + (blocks_read*48) - i) = '\0';			//Strip out the trailing newline inserted by fgets during input sampling
				break;
			}
			else if (*(clear_data + (blocks_read*48) - i) != '\0')			//No newline character
			{
				break;
			}
		}
	}
}

void key_expand()
{
	unsigned char i, j;	//i = row; j = column
	matrix_key = malloc(sizeof(char)*64);
	for (i = 0; i < 8; i++)
	{ 
		for (j = 0; j < 8; j++)
		{
			*(matrix_key + 8*i + j) = (crotl(*(base_key + (i + j)%8),i) & 0x3F) ^ ((i<<3) | j);
		}
	}
}

void encode_base64(const unsigned char* in, unsigned char* out)
{
	*(out + 0) = ((*(in + 0) & 0xFC) >> 2);						//Upper 6 bits of in-0
	*(out + 1) = ((*(in + 0) & 0x03) << 4) | (((*(in + 1) & 0xF0) >> 4) & 0x0F);	//Lower 2 bits of in-0 and upper 4 bits of in-1
	*(out + 2) = ((*(in + 1) & 0x0F) << 2) | (((*(in + 2) & 0xC0) >> 6) & 0x03);	//Lower 4 bits of in-1 and upper 2 bits of in-2
	*(out + 3) = (*(in + 2) & 0x3F);						//Lower 6 bits of in-2
}

unsigned char present_base64(const unsigned char c)
{
	if (c <= 25)
		return c + 0x41;
	else if (c <= 51)
		return c + 0x61 - 26;
	else if (c <= 61)
		return c + 0x30 - 52;
	else if (c == 62)
		return '+';
	else if (c == 63)
		return '/';
	else
		return 0;
}

void encode_message()
{
	int i;
	encoded_data = malloc(blocks_read * 64 + 1);
	memset(encoded_data,0,blocks_read * 64 + 1);
	for(i = 0; i<(16 * blocks_read);i++)
	{
		encode_base64(clear_data + 3*i,encoded_data + 4*i);
	}
}

void encrypt_message()
{
	int i,j;
	for (j = 0; j < blocks_read; j++)
	{
		for (i = 0; i < 64; i++)
		{
			*(encoded_data + j*64 + i) = *(encoded_data + j*64 + i) ^ *(matrix_key + i);
		}
	}
}

void present_message()
{
	int i;
	char temp;
	for(i = 0; i<(blocks_read * 64);i++)
	{
		temp = present_base64(*(encoded_data + i));
		*(encoded_data + i) = temp;
	}
	fprintf(stdout,"%s",encoded_data);
}

int main(int argc, char** argv)
{
	read_data();
	key_expand();
	encode_message();
	encrypt_message();
	present_message();
	return 0;
}
