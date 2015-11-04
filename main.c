#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define MAX 130

int main(int argc, char **agrv) {
	//Encryption table -> table[0]
	//Decryption table -> table[1]
	char table[2][MAX];
	unsigned int index;
	char data;

	memset(table, 0, sizeof(table));
	srand(time(NULL));
	for (index = 'a'; index <= 'z'; index++) {
		data = 'a' + rand() % 26;
		table[0][index] = data;
		table[1][data] = index;
		printf("%c -> %c\n", table[0][index], data);
	}

	for (index = 'A'; index <= 'Z'; index++) {
		data = 'A' + rand() % 26;
		table[0][index] = data;
		table[1][data] = index;
	}

	return 0;
}