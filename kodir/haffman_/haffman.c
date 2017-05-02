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


void search(struct node *tr, char symbol, char* buffer, BoolVector *encoded_s) {

	if ( (*tr).left != NULL ) {
		int index = strlen(buffer);
		buffer[index] = '0';
		buffer[index + 1] = '\0';
		search((*tr).left, symbol, buffer, encoded_s);
	}

	if ( (*tr).right != NULL ) {
		int index = strlen(buffer);
		buffer[index] = '1';
		buffer[index + 1] = '\0';
		search((*tr).right, symbol, buffer, encoded_s);
	}
	
	if( -1 != (*tr).symbol ) {
		//found the symbol
		if( (*tr).symbol == symbol) {
			printf("%s\n", buffer);
			ConcatVector(encoded_s, buffer);
			return;
		}
	}
	
	//like pop back
	int len = strlen(buffer);
	buffer[len - 1] = '\0';
}

void encode1(struct node *tr, char* s, BoolVector *encoded_s) {
	int i = 0;
	printf("pizda %d\n", encoded_s -> length);
	encoded_s -> length = 0;
	char* buffer;
	buffer = malloc( sizeof(char)*8);
	while(s[i]) {
		search(tr, s[i], buffer, encoded_s);
		i++;
	}

	printf("enc = %d\n", encoded_s->length);

	free(buffer);
}	

char* decode(struct node *tr, BoolVector *encoded_s, int i, char* decoded_s) {
	
	if( -1 != (*tr).symbol  ) {
		int len = strlen(decoded_s);
		decoded_s[len] =  (*tr).symbol;
		decoded_s[len+1] = 0;
		return i;
	}

	if(0 == GetValue(encoded_s, i)) {
		return decode((*tr).left, encoded_s, i+1, decoded_s);
	}
	
	if(1 == GetValue(encoded_s,i)) { 
		return decode((*tr).right, encoded_s, i+1, decoded_s);	
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

	BoolVector *encoded_s; 
	encoded_s = malloc( sizeof(BoolVector) );
	encoded_s -> length = 0;
	printf("%d\n", encoded_s -> length);
	
	for(i = 0; i < CAPACITY; i++) {
		encoded_s -> mass[i] = 0;
	}
	
	printf("encode1 вызвался\n");
	encode1(queue[0], s, encoded_s);
	PrintBool(encoded_s);
	
	char decoded_s[encoded_s->length];
	decoded_s[0] = 0;
	i = 0;

	while(i != encoded_s->length){
		i = decode(queue[0], encoded_s, i, decoded_s);
		printf("%d\n", i);
	}

	printf("decoded: %s\n", decoded_s);

	
	free(encoded_s);
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
