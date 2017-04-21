#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "haffman.h"
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

void read_tree(struct node *tr, symbol_code table[], int i[], char *buffer) {

	//типо налево пошли
	if( (*tr).left != NULL ) {
		
		int index = strlen(buffer);
		buffer[index] = '0';
		buffer[index + 1] = '\0';
		read_tree( (*tr).left, table, i, buffer);
	}

	//типо направо пошли
	if( (*tr).right != NULL ) {
		
		int index = strlen(buffer);
		buffer[index] = '1';
		buffer[index + 1] = '\0';
		read_tree( (*tr).right, table, i, buffer);		
	}	
	
	//нашли символ
	if( (*tr).symbol != NULL ) {
		table[*i].symbol = (*tr).symbol;
		strcpy(table[*i].code, buffer);
		*i = *i + 1;
	}
	
	//like pop back
	int len = strlen(buffer);
	buffer[len - 1] = '\0';
	
}

void encode(symbol_code table[], int n, char* s, char* encoded_s) {

	int i, k;
	int len_s = strlen(s);
	
	for(i = 0; i < len_s; i++) {
		//looking for the needed symbol
		for(k = 0; k < n; k++) {
			if(s[i] == table[k].symbol) {
				strncat(encoded_s, table[k].code, strlen(table[k].code));
				break;
			}
		}
	}
	
}

char* decode(struct node *tr, char* encoded_s, char* decoded_s) {

	if( (*tr).symbol != NULL ) {
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
		printf("%d\n", s[i]);
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

		(*cup).symbol = NULL;
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
		//	printf("сдвигал\n");
			queue[i+1] = queue[i];
		}

		//записываем по месту
		queue[k] = cup;
		
		n--;
	}

//дерево в нулевой ячейке
//заполнение таблицы
	symbol_code table[strlen(s)]; //таблица символов

	char buffer[9];
	buffer[0] = '\0';

	int index[1];
	index[0] = 0;

	read_tree(queue[0], table, index,  buffer);
	
	for(i = 0; i < stored_n; i++) {
		printf("s = %d code = %s\n", table[i].symbol, table[i].code);
	}

	char encoded_s[strlen(s)*8]; 
	encoded_s[0] = 0;
	encode(table, stored_n, s, encoded_s);
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

int main() {
	char s[1000];
	printf("Please, input your string:\n");
	scanf("%s", s);
	haffman(s);
}
