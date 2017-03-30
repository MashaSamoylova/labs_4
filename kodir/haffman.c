#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node{
	unsigned char symbol;
	int frequency;
	char code[255];
	struct node *left;
	struct node *right;
};

void free_memory(struct node *fr) {

	if((*fr).left != NULL) {
		free_memory((*fr).left);
	}
	if((*fr).right != NULL) {
		free_memory((*fr).right);
	}

	if(fr != NULL) {
		printf("lol\n");
		free(fr);
		fr = NULL;
	}
}

void QuickSort(struct node *q[], int begin, int end) {
	int mediana = (*q[begin + (end-begin)/2]).frequency;

	int left = begin;
	int right = end;

	while(left <= right) {
		while((*q[left]).frequency > mediana) {
			left++;
		}
		while((*q[right]).frequency < mediana) {
			right--;
		}
		if(left <= right) {
			struct node cup;
			cup.symbol = (*q[left]).symbol;
			cup.frequency = (*q[left]).frequency;

			(*q[left]).symbol = (*q[right]).symbol;
			(*q[left]).frequency = (*q[right]).frequency;

			(*q[right]).symbol = cup.symbol;
			(*q[right]).frequency = cup.frequency;

			left++;
			right--;
		}
	}

	if(left < end) QuickSort(q, left, end);
	if(begin < right) QuickSort(q, begin, right);
}


	//берем два крайних, объединяем в узел и вставляем 
	//продолжаем пока в массиве не останется один элемент

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
				printf("lol %d\n", (*queue[k]).frequency);
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

	printf("n= %d\n", n);

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
		(*cup).left = queue[n-2]; 
		(*cup).right = queue[n-1];


		//теперь все это гавно нужно вставить
		
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
		for(i = n-2; i > k; i--) {
			queue[i+1] = queue[i];
		}

		//записываем по месту
		queue[k] = cup;
		
		n--;

		
	}

	for(i = 0; i < stored_n; i++) {
		//free(queue[i]);
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
