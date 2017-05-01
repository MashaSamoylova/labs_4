#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "my_library.h"

void free_memory(struct node *fr) {

	if( (*fr).left != NULL ) {
		free_memory((*fr).left);
	}
	if( (*fr).right != NULL ) {
		free_memory((*fr).right);
	}

	if(fr != NULL) {
		free(fr);
		fr = NULL;
	}
}

//begining from i position
int write_bytes(char* buffer, char* encoded_s, int i) {
	int block = i / 8;
	int k = 7 - i;
	if (7 == k) {
		encoded_s[block] = 0;
	}
	int index_s;

	for(index_s = 0; buffer[index_s]; index_s++) {
		if(-1 == k) {
			block++;
			encoded_s[block] = 0;
			k = 0;
		}
		if('1' == buffer[index_s]) {
			printf("lol 1\n");
			encoded_s[block] = 1 << k;
			k--;
			i++;
		}
	}
//	encoded_s[block+1] = '\0';
	printf("s = %s\n", encoded_s);
	return i;
}

int search(struct node *tr, char symbol, char* buffer, char* encoded_s, int encode_index) {

	if ( (*tr).left != NULL ) {
		int index = strlen(buffer);
		buffer[index] = '0';
		buffer[index + 1] = '\0';
		search((*tr).left, symbol, buffer, encoded_s, encode_index);
	}

	if ( (*tr).right != NULL ) {
		int index = strlen(buffer);
		buffer[index] = '1';
		buffer[index + 1] = '\0';
		search((*tr).right, symbol, buffer, encoded_s, encode_index);
	}
	
	if( -1 != (*tr).symbol ) {
		//found the symbol
		if( (*tr).symbol == symbol) {
			printf("%s\n", buffer);
			//strncat(encoded_s, buffer, strlen(buffer));
			encode_index = write_bytes(encoded_s, buffer, encode_index);
			return encode_index;
		}
	}
	
	//like pop back
	int len = strlen(buffer);
	buffer[len - 1] = '\0';
}

void encode1(struct node *tr, char* s, int* encoded_s) {
	int i = 0;
	char* buffer;
	buffer = malloc( sizeof(char)*8);
	int encode_index = 0;
	while(s[i]) {
		encode_index = search(tr, s[i], buffer, encoded_s, encode_index);
		i++;
	}

	free(buffer);
}	

char* decode(struct node *tr, char* encoded_s, char* decoded_s) {
	
	if( -1 != (*tr).symbol  ) {
		int len = strlen(decoded_s);
		decoded_s[len] =  (*tr).symbol;
		decoded_s[len+1] = 0;
		return encoded_s;
	}

	if('0' == *encoded_s) {
		return decode((*tr).left, encoded_s + 1, decoded_s);
	}
	
	if('1' == *encoded_s) {
		return decode((*tr).right, encoded_s + 1, decoded_s);	
	}
}


void haffman(char* s) {
	struct node *queue[strlen(s)];

	int i;
	int n = 0;
	for(i = 0; s[i]; i++) {
		int k;
		for(k = 0; k < n; k++) {
			if(s[i] == (*queue[k]).symbol) {
				(*queue[k]).frequency++;
				break;
			}
		}
		if(k == n) {
			queue[k] = malloc( sizeof(struct node) );
			(*queue[k]).symbol = s[i];
			(*queue[k]).frequency = 1;	
			(*queue[k]).left = NULL;
			(*queue[k]).right = NULL;
			n++;
		}
	} //counted the frequency

	for(i = 0; i<n; i++) {
		printf("symbol: %d ", (*queue[i]).symbol);
		printf("f: %d \n", (*queue[i]).frequency);
	}

	printf("n = %d\n", n);

	QuickSort(queue, 0, n-1);

	for(i = 0; i<n; i++) {
		printf("symbol: %d ", (*queue[i]).symbol);
		printf("f: %d \n", (*queue[i]).frequency);
	}

	int stored_n = n;
	
	while(1 != n) {
		//берем две последних ветки
		//объединяем в дерево
		struct node *cup;
		cup = malloc( sizeof(struct node) );
		(*cup).frequency = (*queue[n-1]).frequency + (*queue[n-2]).frequency;
		int cup_frequency = (*cup).frequency;

		(*cup).symbol = -1;
		(*cup).left = queue[n-2];
		(*cup).right = queue[n-1];

		//теперь все это нужно вставить
		
		//ищем место для чашки
		int k;
		for(k = 0; k < n-2; k++) {
			if(cup_frequency <= (*queue[k]).frequency) {
				continue;
			}
			else {
				break;
			}
		}
		
		//сдвигаем хвост
		for(i = n-3; k <= i; i--) {
			queue[i+1] = queue[i];
		}

		//записываем по месту
		queue[k] = cup;
		
		n--;
	}

//дерево в нулевой ячейке

	char buffer[9];
	buffer[0] = '\0';

//	read_tree(queue[0], table, index,  buffer);
	
/*	for(i = 0; i < stored_n; i++) {
		printf("s = %d code = %s\n", table[i].symbol, table[i].code);
	}
*/
	int encoded_s[strlen(s)]; 
	encoded_s[0] = 0;
	//encode(table, stored_n, s, encoded_s);
	encode1(queue[0], s, encoded_s);
	printf("encoded: %s", encoded_s);
	
	char decoded_s[strlen(encoded_s)];
	decoded_s[0] = 0;
	char* pointer = encoded_s;

	while(0 != *pointer){
		pointer = decode(queue[0], pointer, decoded_s);
		printf("%s\n", pointer);
	}

	printf("decoded: %s\n", decoded_s);

	for(i = 0; i < stored_n; i++) {
		if(queue[i] != NULL) {
			free_memory(queue[i]);
		}
	}

}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("please, use ./haffman <filename>\n");
		return 0;
	}

	FILE *fd;
	fd = fopen(argv[1], "r");
	if(fd == NULL) {
		printf("SOME ERROR:(\n");
		return 0;
	}

	char* s = malloc( sizeof(char)*1000 );
	
	int i = 0;
	do {
		s[i] = fgetc(fd);
		if(s[i] == EOF) {
			if(feof(fd)) {
				printf("Считывание завершено\n");
				break;
			}
		}
		i++;
	} while(1);
	s[i] = '\0';
	
	printf("считанное говно = %s\n", s);
	haffman(s);
	fclose(fd);
	free(s);
	return 0;
}
